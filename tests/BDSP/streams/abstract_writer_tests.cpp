#include <gtest/gtest.h>

#include "../../utils/testing.h"
#include "BDSP/streams/abstract/writer.h"

using namespace BDSP::streams;

class SimpleWriter: public ABS::AbstractWriter {
    void _process_byte(uint8_t byte) override { _write(byte); };
    void _finish() override { _write(end_header.data(), end_header.size()); };

public:
    std::vector<uint8_t> end_header = {0xFF, 0xFF};

    void toggle_ready() { _set_ready_state(not get_ready_status()); }
};

TEST(abstract_writer_tests, abstract_writer_errors_test) {
    auto writer = SimpleWriter();
    auto status = writer.write(0x00);
    EXPECT_EQ(status, ERROR_WRITE_STREAM_NOT_READY);

    status = writer.finish();
    EXPECT_EQ(status, ERROR_WRITE_STREAM_NOT_READY);

    std::vector<uint8_t> data = {0};
    status = writer.write(data.data(), data.size());
    EXPECT_EQ(status, ERROR_WRITE_STREAM_NOT_READY);

    writer.set_stream_writer([](uint8_t byte, void *ctx) { }, nullptr);

    writer.toggle_ready();

    status = writer.write(0x00);
    EXPECT_EQ(status, ERROR_WRITE_STREAM_NOT_READY);

    status = writer.finish();
    EXPECT_EQ(status, ERROR_WRITE_STREAM_NOT_READY);
}

TEST(abstract_writer_tests, abstract_writer_normal_using_test) {
    auto writer = SimpleWriter();

    std::vector<uint8_t> data = {0};

    writer.set_stream_writer([](uint8_t byte, void *ctx) { }, nullptr);

    auto status = writer.write(data.data(), data.size());
    EXPECT_EQ(status, STREAM_READ_OK);

    status = writer.finish();
    EXPECT_EQ(status, STREAM_READ_END);
}

TEST(abstract_writer_tests, abstract_writer_write_test) {
    auto writer = SimpleWriter();

    std::vector<uint8_t> data = {0x00};

    writer.set_stream_writer([](uint8_t byte, void *ctx) { EXPECT_EQ(byte, 0x00); }, nullptr);
    auto status = writer.write(data[0]);
    EXPECT_EQ(status, STREAM_READ_OK);

    std::vector<uint8_t> output;
    writer.set_stream_writer(
        [](uint8_t byte, void *ctx) { reinterpret_cast<std::vector<uint8_t> *>(ctx)->push_back(byte); }, &output);

    status = writer.finish();
    EXPECT_EQ(status, STREAM_READ_END);

    std::vector<uint8_t> correct_encoded = data;
    correct_encoded.insert(correct_encoded.end(), writer.end_header.begin(), writer.end_header.end());
    start_test_writer(writer, data, correct_encoded);
}