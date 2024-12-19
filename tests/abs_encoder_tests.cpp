#include <cstdint>
#include <gtest/gtest.h>

#include <BDSP/encoders/abs_encoder.h>
#include "utils/testing.h"

using namespace BDSP::encoders::ABS;

class SimpleEncoder : public AbstractEncoder {
    void _encode(uint8_t byte) override {
        _write(byte);
    };

    void _finish_encode() override {
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
    auto status = encoder.encode(0x00);
    EXPECT_EQ(status, UNKNOWN_ENCODER_ERROR);

    status = encoder.finish_encode();
    EXPECT_EQ(status, UNKNOWN_ENCODER_ERROR);

    std::vector<uint8_t> data = {0};
    status = encoder.encode(data.data(), data.size());
    EXPECT_EQ(status, UNKNOWN_ENCODER_ERROR);

    encoder.set_writer([](uint8_t byte, void *ctx) {});

    encoder.toggle_ready();

    status = encoder.encode(0x00);
    EXPECT_EQ(status, UNKNOWN_ENCODER_ERROR);

    status = encoder.finish_encode();
    EXPECT_EQ(status, UNKNOWN_ENCODER_ERROR);
}

TEST(abstract_encoder_tests, abstract_encoder_normal_using_test) {
    auto encoder = SimpleEncoder();

    std::vector<uint8_t> data = {0};

    encoder.set_writer([](uint8_t byte, void *ctx) {});

    auto status = encoder.encode(data.data(), data.size());
    EXPECT_EQ(status, DECODE_OK);

    status = encoder.finish_encode();
    EXPECT_EQ(status, DECODE_END);
}

TEST(abstract_encoder_tests, abstract_encoder_write_test) {
    auto encoder = SimpleEncoder();

    std::vector<uint8_t> data = {0x00};

    encoder.set_writer([](uint8_t byte, void *ctx) { EXPECT_EQ(byte, 0x00); });
    auto status = encoder.encode(data[0]);
    EXPECT_EQ(status, DECODE_OK);

    std::vector<uint8_t> output;
    encoder.set_writer([](uint8_t byte, void *ctx) {
        reinterpret_cast<std::vector<uint8_t> *>(ctx)->push_back(byte);
    }, &output);

    status = encoder.finish_encode();
    EXPECT_EQ(status, DECODE_END);

    std::vector<uint8_t> correct_encoded = data;
    correct_encoded.insert(correct_encoded.end(), encoder.end_header.begin(), encoder.end_header.end());
    start_test_encoder(encoder, data, correct_encoded);
}