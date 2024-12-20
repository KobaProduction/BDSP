#include <cstdint>
#include <gtest/gtest.h>

#include <BDSP/decoders/abs_decoder.h>
#include "utils/testing.h"

using namespace BDSP::decoders::ABS;

class SimpleDecoder : public AbstractDecoder {
    decode_status_t _decode(uint8_t byte) override {
        decode_status_t status = DECODE_OK;
        if (byte == error_byte) return DECODE_ERROR;
        if (byte == end_byte) return DECODE_END;
        _handler(byte, status);
        byte_counter++;
        return status;
    }

    void _reset() override {
        byte_counter = 0;
    };
public:
    uint8_t error_byte = 0xF0;
    uint8_t end_byte = 0xFF;

    size_t byte_counter = 0;
};

TEST(abstract_decoder_tests, abstract_decoder_errors_test) {
    auto decoder = SimpleDecoder();
    auto status = decoder.decode(0x00);
    EXPECT_EQ(status, UNKNOWN_DECODER_ERROR);

    status = decoder.decode(&decoder.error_byte, 1);
    EXPECT_EQ(status, UNKNOWN_DECODER_ERROR);

    decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx) {
        EXPECT_EQ(status, DECODE_ERROR);
    });

    status = decoder.decode(decoder.error_byte);
    EXPECT_EQ(status, DECODE_ERROR);

    status = decoder.decode(&decoder.error_byte, 1);
    EXPECT_EQ(status, DECODE_ERROR);
}

TEST(abstract_decoder_tests, abstract_decoder_normal_using_test) {
    auto decoder = SimpleDecoder();

    auto normal_handler = [](uint8_t byte, decode_status_t status, void *ctx) {
        EXPECT_EQ(byte, 0x00);
        EXPECT_EQ(status, DECODE_OK);
        *reinterpret_cast<bool *>(ctx) = true;
    };

    bool handler_used = false;
    decoder.set_data_handler(normal_handler, &handler_used);

    auto status = decoder.decode(0x00);
    EXPECT_EQ(status, DECODE_OK);
    EXPECT_EQ(handler_used, true);

    EXPECT_EQ(decoder.byte_counter, 1);
    decoder.reset_decode_state(false);
    EXPECT_EQ(decoder.byte_counter, 0);

    std::vector<uint8_t> data = {0x00, 0x00, 0x00};
    status = decoder.decode(data.data(), data.size());
    EXPECT_EQ(status, DECODE_OK);
    EXPECT_EQ(decoder.byte_counter, data.size());

    handler_used = false;
    decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx) {
        *reinterpret_cast<bool *>(ctx) = true;
        EXPECT_EQ(status, DECODE_END);
    }, &handler_used);

    status = decoder.decode(decoder.end_byte);
    EXPECT_EQ(handler_used, true);
    EXPECT_EQ(status, DECODE_END);
    EXPECT_EQ(decoder.byte_counter, 0);

    handler_used = false;
    decoder.set_data_handler(normal_handler, &handler_used);
    status = decoder.decode(0x00);
    EXPECT_EQ(handler_used, true);
    EXPECT_EQ(status, DECODE_OK);

}

TEST(abstract_decoder_tests, abstract_decoder_read_correct_test) {
    struct Context {
        SimpleDecoder decoder;
        std::vector<uint8_t> decoded;
        std::vector<uint8_t> correct = {0x00, 0x00};
        std::vector<uint8_t> encoded = correct;
        int number_of_end_decodes = 0;
    } ctx;

    ctx.encoded.push_back(ctx.decoder.end_byte);

    ctx.decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx) {
        reinterpret_cast<std::vector<uint8_t> *>(ctx)->push_back(byte);
    }, &ctx.decoded);

    start_test_decoder(ctx.decoder, ctx.encoded, ctx.correct);
    EXPECT_EQ(ctx.decoder.byte_counter, 0);

    ctx.decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx) {
        auto &context = *reinterpret_cast<Context *>(ctx);
        if (status == DECODE_OK) {
            context.decoded.push_back(byte);
        } else if (status == DECODE_END) {
            context.number_of_end_decodes++;
            EXPECT_EQ(byte, context.decoder.end_byte);
            EXPECT_EQ(is_equals(context.decoded, context.correct), true);
            context.decoded.clear();
        } else {
            FAIL() << "An implicit error has occurred.";
        }

    }, &ctx);

    ctx.decoded.clear();
    ctx.encoded.insert(ctx.encoded.end(), ctx.correct.begin(), ctx.correct.end());
    ctx.encoded.push_back(ctx.decoder.end_byte);
    auto status = ctx.decoder.decode(ctx.encoded.data(), ctx.encoded.size());
    EXPECT_EQ(status, DECODE_OK);
    EXPECT_EQ(ctx.number_of_end_decodes, 2);
}

TEST(abstract_decoder_tests, abstract_decoder_read_errors_test) {
    struct Context {
        SimpleDecoder decoder;
        std::vector<uint8_t> decoded;
        std::vector<uint8_t> correct = {0x00, 0x00};
        std::vector<uint8_t> encoded = correct;
        int number_of_end_decodes = 0;
        int number_of_error_decodes = 0;
    } ctx;

    ctx.encoded.push_back(ctx.decoder.error_byte);

    ctx.decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx) {
        auto &context = *reinterpret_cast<Context *>(ctx);
        if (status == DECODE_OK) {
            context.decoded.push_back(byte);
        } else if (status == DECODE_END) {
            context.number_of_end_decodes++;
            EXPECT_EQ(byte, context.decoder.end_byte);
        } else if (status == DECODE_ERROR) {
            EXPECT_EQ(context.number_of_error_decodes, 0);
            context.number_of_error_decodes++;
            EXPECT_EQ(context.decoder.byte_counter, 2);
            EXPECT_EQ(byte, context.decoder.error_byte);
            context.decoded.clear();
        } else {
            FAIL() << "An implicit error has occurred.";
        }
    }, &ctx);

    auto status = ctx.decoder.decode(ctx.encoded.data(), ctx.encoded.size());
    EXPECT_EQ(status, DECODE_ERROR);

    EXPECT_EQ(ctx.decoded.size(), 0);

    auto new_correct = ctx.correct;
    new_correct.insert(new_correct.end(), ctx.correct.begin(), ctx.correct.end());

    ctx.encoded.push_back(ctx.decoder.end_byte);
    ctx.encoded.insert(ctx.encoded.end(), new_correct.begin(), new_correct.end());
    ctx.encoded.push_back(ctx.decoder.end_byte);

    ctx.number_of_error_decodes = 0;
    status = ctx.decoder.decode(ctx.encoded.data(), ctx.encoded.size());
    EXPECT_EQ(status, DECODE_ERROR);
    EXPECT_EQ(is_equals(ctx.decoded, new_correct), true);
    EXPECT_EQ(ctx.number_of_end_decodes, 2);

}