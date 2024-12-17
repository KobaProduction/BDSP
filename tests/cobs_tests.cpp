#include <cstdint>
#include <gtest/gtest.h>

#include <BDSP/encoders/COBS.h>
#include <BDSP/decoders/COBS.h>

#include "utils/cobs.h"
#include "utils/testing.h"


TEST(cobs_encoding_test, encoding_test) {
    BDSP::encoders::COBS::COBSEncoder cobs_encoder;

    std::vector<uint8_t> data;
    std::vector<uint8_t> correct_encoded;

    for (int size = 1; size < 1000; ++size) {
        data.clear();
        correct_encoded.clear();

        for (int i = 0; i < size; ++i) data.push_back(i);
        cobs_encode(data, correct_encoded);
        start_test_encoder(cobs_encoder, data, correct_encoded);
    }
}

TEST(cobs_encoding_test, decoding_test) {
    BDSP::decoders::COBS::COBSDecoder decoder;

    std::vector<uint8_t> data;
    std::vector<uint8_t> correct_encoded;

    for (int size = 1; size < 1000; ++size) {
        data.clear();
        correct_encoded.clear();
        for (int i = 0; i < size; ++i) data.push_back(i);
        cobs_encode(data, correct_encoded);
        start_test_decoder(decoder, correct_encoded, data);
    }
}
