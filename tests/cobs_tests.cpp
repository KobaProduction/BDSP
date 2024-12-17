#include <cstdint>
#include <gtest/gtest.h>

#include <BDSP/encoders/COBS.h>
#include <BDSP/decoders/COBS.h>

#include "utils/cobs.h"
#include "utils/testing.h"
#include "utils/show.h"

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

TEST(cobs_encoding_test, encoding_test) {
    BDSP::encoders::COBS::COBSEncoder encoder;

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

TEST(cobs_encoding_test, decoding_test) {
    BDSP::decoders::COBS::COBSDecoder decoder;

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
