#ifndef BDSP_TESTS_UTILS_TESTING_H
#define BDSP_TESTS_UTILS_TESTING_H

#include <cstdint>
#include <vector>

#include <BDSP/encoders/abs_encoder.h>
#include <BDSP/decoders/abs_decoder.h>

#include "show.h"

bool is_equals(const uint8_t *data1, size_t size1, const uint8_t *data2, size_t size2);

bool is_equals(const uint8_t *data1, size_t size1, std::vector<uint8_t> data2);

bool is_equals(std::vector<uint8_t> data1, std::vector<uint8_t> data2);


void start_test_encoder(
    BDSP::encoders::ABS::AbstractEncoder &encoder,
    std::vector<uint8_t> &data,
    std::vector<uint8_t> &correct_encoded,
    show_t type = DEC
);

void start_test_decoder(
        BDSP::decoders::ABS::AbstractDecoder &decoder,
        std::vector<uint8_t> &encoded,
        std::vector<uint8_t> &correct_decoded,
        show_t type = DEC
);

#endif //BDSP_TESTS_UTILS_TESTING_H
