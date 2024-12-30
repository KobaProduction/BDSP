#include <cstdint>
#include <gtest/gtest.h>

#include <BDSP/streams/abstract/writer.h>
#include "utils/testing.h"

using namespace BDSP::streams;

class SimpleEncoder : public ABS::AbstractWriter {
    void _process_byte(uint8_t byte) override {
        _write(byte);
    };

    void _finish() override {
        _write(end_header.data(), end_header.size());
    };
public:
    std::vector<uint8_t> end_header = {0xFF, 0xFF};

    void toggle_ready() {
        _is_ready = !_is_ready;
    }
};

TEST(abstract_encoder_tests, abstract_encoder_errors_test) {
    auto encoder = SimpleEncoder();
    auto status = encoder.write(0x00);
    EXPECT_EQ(status, UNKNOWN_WRITER_ERROR);

    status = encoder.finish();
    EXPECT_EQ(status, UNKNOWN_WRITER_ERROR);

    std::vector<uint8_t> data = {0};
    status = encoder.write(data.data(), data.size());
    EXPECT_EQ(status, UNKNOWN_WRITER_ERROR);

    encoder.set_writer([](uint8_t byte, void *ctx) {}, nullptr);

    encoder.toggle_ready();

    status = encoder.write(0x00);
    EXPECT_EQ(status, UNKNOWN_WRITER_ERROR);

    status = encoder.finish();
    EXPECT_EQ(status, UNKNOWN_WRITER_ERROR);
}

TEST(abstract_encoder_tests, abstract_encoder_normal_using_test) {
    auto encoder = SimpleEncoder();

    std::vector<uint8_t> data = {0};

    encoder.set_writer([](uint8_t byte, void *ctx) {}, nullptr);

    auto status = encoder.write(data.data(), data.size());
    EXPECT_EQ(status, READ_OK);

    status = encoder.finish();
    EXPECT_EQ(status, READ_END);
}

TEST(abstract_encoder_tests, abstract_encoder_write_test) {
    auto encoder = SimpleEncoder();

    std::vector<uint8_t> data = {0x00};

    encoder.set_writer([](uint8_t byte, void *ctx) { EXPECT_EQ(byte, 0x00); }, nullptr);
    auto status = encoder.write(data[0]);
    EXPECT_EQ(status, READ_OK);

    std::vector<uint8_t> output;
    encoder.set_writer([](uint8_t byte, void *ctx) {
        reinterpret_cast<std::vector<uint8_t> *>(ctx)->push_back(byte);
    }, &output);

    status = encoder.finish();
    EXPECT_EQ(status, READ_END);

    std::vector<uint8_t> correct_encoded = data;
    correct_encoded.insert(correct_encoded.end(), encoder.end_header.begin(), encoder.end_header.end());
    start_test_writer(encoder, data, correct_encoded);
}