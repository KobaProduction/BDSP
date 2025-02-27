#include <gtest/gtest.h>

#include "cobs.h"
#include "testing.h"

TEST(cobs_utils_tests, cobs_reference_encoder_test) {
    std::vector<uint8_t> data = {5, 1, 0, 1};
    std::vector<uint8_t> encoded;
    std::vector<uint8_t> correct_encoded = {3, 5, 1, 2, 1, 0};

    cobs_encode(data, encoded);
    check_data_for_correctness(encoded, correct_encoded);
}

TEST(cobs_utils_tests, cobs_reference_decoder_test) {
    std::vector<uint8_t> encoded = {3, 5, 1, 2, 1, 0};
    std::vector<uint8_t> decoded;
    std::vector<uint8_t> correct_decoded = {5, 1, 0, 1};

    cobs_decode(encoded, decoded);
    check_data_for_correctness(decoded, correct_decoded);
}

TEST(cobs_utils_tests, cobs_reference_encoders_test) {
    std::vector<uint8_t> data;
    std::vector<uint8_t> encoded;
    std::vector<uint8_t> decoded;

    for (int i = 0; i < 512; ++i) {
        data.push_back(i);
    }
    cobs_encode(data, encoded);
    cobs_decode(encoded, decoded);
    check_data_for_correctness(data, decoded);
}
