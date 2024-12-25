#include <cstdint>
#include <vector>
#include <gtest/gtest.h>

#include <BDSP/encoders/PPP.h>
#include <BDSP/decoders/PPP.h>

#include "utils/show.h"
#include "utils/testing.h"

using namespace BDSP::encoders::PPP;
using namespace BDSP::decoders::PPP;


TEST(ppp_encoding_test, encoding_test) {
    PPPEncoder encoder;

    std::vector<uint8_t> data = {0x7E, 0x7D, 0x00};
    std::vector<uint8_t> correct_encoded = {0x7D, 0x5E, 0x7D, 0x5D, 0x00, 0x7E};

    start_test_encoder(encoder, data, correct_encoded, HEX);
}

TEST(ppp_encoding_test, decoding_errors_test) {
    struct Context {
        PPPDecoder decoder;
        std::vector<uint8_t> decoded;
        std::vector<uint8_t> encoded;
    } ctx;

    ctx.decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx) {
        if (status == DECODE_OK) reinterpret_cast<Context *>(ctx)->decoded.push_back(byte);
    }, &ctx);

    for (uint8_t byte = 0; byte < 255; ++byte) {
        ctx.decoder.reset_decode_state(false);
        ctx.decoded.clear();
        ctx.encoded = {0x7D, byte};
        auto status = ctx.decoder.decode(ctx.encoded.data(), ctx.encoded.size());

        if (byte not_eq 0x5D and byte not_eq 0x5E) {
            EXPECT_EQ(ctx.decoded.size(), 0);
        } else {
            EXPECT_EQ(status, DECODE_OK);
            EXPECT_EQ(ctx.decoded.size(), 1);
            EXPECT_EQ(ctx.decoded[0], byte ^ 0x20);
        }
    }

}

TEST(ppp_encoding_test, decoding_normal_test) {
    struct Context {
        PPPDecoder decoder;
        std::vector<uint8_t> encoded;
        std::vector<uint8_t> decoded;
        std::vector<uint8_t> correct;
    } ctx;

    ctx.encoded = {0x00, 0x01, 0x7E};
    ctx.correct = {0x00, 0x01};
    start_test_decoder(ctx.decoder, ctx.encoded, ctx.correct);

    ctx.decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx) {
        auto &context = *reinterpret_cast<Context *>(ctx);
        switch (status) {
            case DECODE_OK:
                context.decoded.push_back(byte);
                break;
            case DECODE_END:
                break;
            case DECODE_ERROR:
            case UNKNOWN_DECODER_ERROR:
                FAIL() << "An unforeseen mistake arose";
        }
    }, &ctx);

    ctx.encoded = {0x7E, 0x7E, 0x00, 0x01, 0x7E};
    ctx.correct = {0x00, 0x01};
    ctx.decoder.decode(ctx.encoded.data(), ctx.encoded.size());
    EXPECT_EQ(is_equals(ctx.decoded, ctx.correct), true);
}