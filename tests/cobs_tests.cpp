#include <cstdint>
#include <gtest/gtest.h>

#include <BDSP/encoders/COBS.h>
#include <BDSP/decoders/COBS.h>

#include "utils/cobs.h"
#include "utils/testing.h"
#include "utils/show.h"

using namespace BDSP::encoders::COBS;
using namespace BDSP::decoders::COBS;

TEST(cobs_encoding_test, cobs_utils_test) {
    std::vector<uint8_t> data;
    std::vector<uint8_t> encoded;
    std::vector<uint8_t> decoded;

    for (int i = 0; i < 512; ++i) data.push_back(i);
    cobs_encode(data, encoded);
    cobs_decode(encoded, decoded);
    if (is_equals(data, decoded)) return;

    std::cout << "Correct ";
    show_data(data);
    std::cout << "Encoded ";
    show_data(encoded);
    std::cout << "Decoded ";
    show_data(decoded);
    FAIL() << "the correct and encoded array is not equal";
}

TEST(cobs_encoding_test, encoding_default_test) {
    COBSEncoder encoder;

    std::vector<uint8_t> data;
    std::vector<uint8_t> correct_encoded;

    for (int size = 1; size < 1000; ++size) {
        data.clear();
        correct_encoded.clear();

        for (int i = 0; i < size; ++i) data.push_back(i);
        cobs_encode(data, correct_encoded);
        start_test_encoder(encoder, data, correct_encoded);
    }
}

TEST(cobs_encoding_test, encoding_custom_config_test) {
    COBSEncoder encoder({.delimiter = '\n'});
    COBSDecoder decoder({.delimiter = '\n'});

    struct Context {
        std::vector<uint8_t> data;
        std::vector<uint8_t> encoded;
        std::vector<uint8_t> decoded;
        bool is_ended = false;
    } ctx;

    encoder.set_writer([] (uint8_t byte, void *ctx) {
        reinterpret_cast<Context*>(ctx)->encoded.push_back(byte);
    }, &ctx);

    decoder.set_data_handler([] (uint8_t byte, decode_status_t status, void *ctx) {
        auto &context = *reinterpret_cast<Context*>(ctx);
        ASSERT_FALSE(context.is_ended);
        if (status == DECODE_OK) context.decoded.push_back(byte);
        if (status == DECODE_END) context.is_ended = true;
    }, &ctx);

    for (int size = 1; size < 1000; ++size) {
        ctx.data.clear();
        ctx.encoded.clear();
        ctx.decoded.clear();

        for (int i = 0; i < size; ++i) ctx.data.push_back(i);
        encoder.encode(ctx.data.data(), ctx.data.size());
        encoder.finish_encode();
        decoder.decode(ctx.encoded.data(), ctx.encoded.size());
        ASSERT_TRUE(is_equals(ctx.data, ctx.decoded));
        ASSERT_TRUE(ctx.is_ended);
        ctx.is_ended = false;
    }
}

TEST(cobs_encoding_test, decoding_test) {
    COBSDecoder decoder;

    std::vector<uint8_t> data;
    std::vector<uint8_t> encoded;

    for (int size = 1; size < 1000; ++size) {
        data.clear();
        encoded.clear();
        for (int i = 0; i < size; ++i) data.push_back(i);
        cobs_encode(data, encoded);
        start_test_decoder(decoder, encoded, data);
    }
}
