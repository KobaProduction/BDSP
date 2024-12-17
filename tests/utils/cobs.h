#ifndef BDSP_TESTS_UTILS_COBS_H
#define BDSP_TESTS_UTILS_COBS_H

#include <cstdint>
#include <vector>

uint8_t *cobs_decode(const uint8_t *ptr, size_t length, uint8_t *dst);

void cobs_decode(std::vector<uint8_t> &encoded_buffer, std::vector<uint8_t> &output_buffer);

uint8_t *cobs_encode(const uint8_t *ptr, size_t length, uint8_t *dst);

void cobs_encode(std::vector<uint8_t> &data, std::vector<uint8_t> &encoded_buffer);

#endif //BDSP_TESTS_UTILS_COBS_H
