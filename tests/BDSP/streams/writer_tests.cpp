#include <gtest/gtest.h>

#include "../../utils/testing.h"

#include "BDSP/streams/abstract/writer.h"
#include "BDSP/streams/writer.h"

using namespace BDSP::streams;

namespace {
std::vector<uint8_t> finish_bytes = {0xFF, 0xFF};

class TestAbstractStreamWriteStrategy final: public core::AbstractStreamWriteStrategy {
public:
    void write(uint8_t byte) override { _write_handler(byte, _context); }

    void send_delimiter() override {
        for (unsigned char i: finish_bytes) {
            _write_handler(i, _context);
        }
    }
};

void write_handler(uint8_t byte, void *ctx) {
    reinterpret_cast<std::vector<uint8_t> *>(ctx)->push_back(byte);
}
} // namespace

TEST(stream_writer_tests, ready_test) {
    class TestStreamWriter: public core::StreamWriterCore {
    public:
        using StreamWriterCore::_set_strategy;
    };
    auto writer = TestStreamWriter();
    writer.set_stream_writer(nullptr, nullptr);

    ASSERT_FALSE(writer.get_ready_status());

    auto strategy = TestAbstractStreamWriteStrategy();
    writer._set_strategy(strategy);

    ASSERT_FALSE(writer.get_ready_status());

    writer.set_stream_writer([](uint8_t byte, void *ctx) { }, nullptr);
    ASSERT_TRUE(writer.get_ready_status());
}

TEST(stream_writer_tests, finish_test) {
    auto writer = StreamWriter<TestAbstractStreamWriteStrategy>();

    ASSERT_FALSE(writer.get_ready_status());

    EXPECT_EQ(writer.finish(), STREAM_WRITER_NOT_READY_ERROR);

    std::vector<uint8_t> stream;

    writer.set_stream_writer(write_handler, &stream);

    ASSERT_TRUE(writer.get_ready_status());

    EXPECT_EQ(writer.finish(), STREAM_WRITE_END);
    ASSERT_TRUE(stream == finish_bytes);
}

TEST(stream_writer_tests, write_test) {
    auto writer = StreamWriter<TestAbstractStreamWriteStrategy>();

    ASSERT_FALSE(writer.get_ready_status());

    EXPECT_EQ(writer.write(0), STREAM_WRITER_NOT_READY_ERROR);
    std::vector<uint8_t> stream;

    writer.set_stream_writer(write_handler, &stream);
    EXPECT_EQ(writer.write(0), STREAM_WRITE_OK);
    ASSERT_TRUE(stream.size() == 1 and stream[0] == 0);
}

TEST(stream_writer_tests, write_array_test) {
    auto writer = StreamWriter<TestAbstractStreamWriteStrategy>();

    std::vector<uint8_t> data = {0, 1, 2};

    ASSERT_FALSE(writer.get_ready_status());

    EXPECT_EQ(writer.write(data.data(), data.size()), STREAM_WRITER_NOT_READY_ERROR);
    std::vector<uint8_t> stream;

    writer.set_stream_writer(write_handler, &stream);
    EXPECT_EQ(writer.write(data.data(), data.size()), STREAM_WRITE_OK);
    ASSERT_TRUE(stream == data);
}