#include <gtest/gtest.h>

#include "../../utils/testing.h"

#include "BDSP/streams/abstract/reader.h"
#include "BDSP/streams/reader.h"

using namespace BDSP::streams;

namespace {

std::vector<uint8_t> finish_bytes = {0xFF, 0xFF};

class EmptyStrategy final: public core::AbstractStreamReadStrategy {
public:
    read_status_t read(uint8_t byte) override {
        read_status_t status = byte == 0 ? STREAM_READ_ERROR : byte == 1 ? STREAM_READ_END : STREAM_READ_OK;
        _read_callback(byte, status, _context);
        return status;
    };

    void reset_read_state() override {

    };
};

struct Context {
    std::vector<uint8_t> data;
    std::vector<read_status_t> statuses;
};

void read_handler(uint8_t byte, read_status_t status, void *ctx) {
    reinterpret_cast<Context *>(ctx)->data.push_back(byte);
    reinterpret_cast<Context *>(ctx)->statuses.push_back(status);
}
} // namespace

TEST(stream_reader_tests, ready_test) {
    class TestStreamReader: public core::StreamReaderCore {
    public:
        using StreamReaderCore::_set_strategy;
    };
    auto reader = TestStreamReader();

    ASSERT_FALSE(reader.get_ready_status());

    auto strategy = EmptyStrategy();
    reader._set_strategy(strategy);

    ASSERT_FALSE(reader.get_ready_status());

    reader.set_stream_data_handler([](uint8_t byte, read_status_t status, void *ctx) { }, nullptr);
    ASSERT_TRUE(reader.get_ready_status());
}

TEST(stream_reader_tests, read_test) {
    auto reader = StreamReader<EmptyStrategy>();

    ASSERT_FALSE(reader.get_ready_status());

    Context context;

    reader.set_stream_data_handler(read_handler, &context);
    EXPECT_EQ(reader.read(2), STREAM_READ_OK);
    ASSERT_TRUE(context.data.size() == 1 and context.data[0] == 2);
    ASSERT_TRUE(context.statuses.size() == 1 and context.statuses[0] == STREAM_READ_OK);

    context.data.clear();
    context.statuses.clear();

    EXPECT_EQ(reader.read(1), STREAM_READ_END);
    ASSERT_TRUE(context.data.size() == 1 and context.data[0] == 1);
    ASSERT_TRUE(context.statuses.size() == 1 and context.statuses[0] == STREAM_READ_END);

    context.data.clear();
    context.statuses.clear();

    EXPECT_EQ(reader.read(0), STREAM_READ_ERROR);
    ASSERT_TRUE(context.data.size() == 1 and context.data[0] == 0);
    ASSERT_TRUE(context.statuses.size() == 1 and context.statuses[0] == STREAM_READ_ERROR);
}

TEST(stream_reader_tests, read_when_not_ready_test) {
    auto reader = StreamReader<EmptyStrategy>();
    EXPECT_EQ(reader.read(0), STREAM_READER_NOT_READY_ERROR);

    std::vector<uint8_t> stream = {0, 1, 2};
    EXPECT_EQ(reader.read(stream.data(), stream.size()), STREAM_READER_NOT_READY_ERROR);
}

TEST(stream_reader_tests, read_when_have_errors_test) {
    auto reader = StreamReader<EmptyStrategy>();
    Context context;
    reader.set_stream_data_handler(read_handler, &context);
    std::vector<uint8_t> stream = {3, 3, 0, 2, 3, 3, 3};
    EXPECT_EQ(reader.read(stream.data(), stream.size()), STREAM_READ_ERROR);
    ASSERT_TRUE(context.data == std::vector<uint8_t>({3, 3, 0}));
    ASSERT_TRUE(context.statuses == std::vector<read_status_t>({STREAM_READ_OK, STREAM_READ_OK, STREAM_READ_ERROR}));
}

TEST(stream_reader_tests, manual_reset_after_error__test) {
    auto reader = StreamReader<EmptyStrategy>();
    Context context;
    reader.set_stream_data_handler(read_handler, &context);

    EXPECT_EQ(reader.read(0), STREAM_READ_ERROR);
    reader.reset_read_state(false);
    EXPECT_EQ(reader.read(3), STREAM_READ_OK);
    ASSERT_TRUE(context.data == std::vector<uint8_t>({0, 3}));
    ASSERT_TRUE(context.statuses == std::vector<read_status_t>({STREAM_READ_ERROR, STREAM_READ_OK}));
}

TEST(stream_reader_tests, reset_when_have_errors_and_read_delimiter_test) {
    auto reader = StreamReader<EmptyStrategy>();
    Context context;
    reader.set_stream_data_handler(read_handler, &context);
    std::vector<uint8_t> stream = {3, 0, 3, 1, 3};
    EXPECT_EQ(reader.read(stream.data(), stream.size()), STREAM_READ_ERROR);
    ASSERT_TRUE(context.data == std::vector<uint8_t>({3, 0, 1, 3}));
    ASSERT_TRUE(context.statuses == std::vector<read_status_t>({STREAM_READ_OK, STREAM_READ_ERROR, STREAM_READ_END, STREAM_READ_OK}));
}

