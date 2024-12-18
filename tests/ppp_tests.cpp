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
    BDSP::encoders::PPP::PPPEncoder encoder;

    std::vector<uint8_t> data = {0x7E, 0x7D, 0x00};
    std::vector<uint8_t> correct_encoded = {0x7D, 0x5E, 0x7D, 0x5D, 0x00, 0x7E};

    start_test_encoder(encoder, data, correct_encoded, HEX);
}

TEST(ppp_encoding_test, decoding_test) {
    BDSP::decoders::PPP::PPPDecoder decoder;

    std::vector<uint8_t> data = {0x7E, 0x7D, 0x00};
    std::vector<uint8_t> encoded = {0x7D, 0x5E, 0x7D, 0x5D, 0x00, 0x7E};

    start_test_decoder(decoder, encoded, data, HEX);
}

TEST(ppp_encoding_test, decoding_error_test) {
    BDSP::decoders::PPP::PPPDecoder decoder;

    decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx_ptr) {});

    std::vector<uint8_t> correct_encoded = {0x7E};
    auto status = decoder.decode(correct_encoded[0]);
    EXPECT_EQ(status, DECODE_END);

    correct_encoded = {0x7D, 0x7D};
    status = decoder.decode(correct_encoded.data(), correct_encoded.size());
    EXPECT_EQ(status, DECODE_ERROR);

    correct_encoded = {0x7D, 0x5D};
    status = decoder.decode(correct_encoded.data(), correct_encoded.size());
    EXPECT_EQ(status, DECODE_OK);

    correct_encoded = {0x7D, 0x5E};
    status = decoder.decode(correct_encoded.data(), correct_encoded.size());
    EXPECT_EQ(status, DECODE_OK);

}