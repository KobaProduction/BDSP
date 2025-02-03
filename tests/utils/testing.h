#ifndef BDSP_TESTS_UTILS_TESTING_H
#define BDSP_TESTS_UTILS_TESTING_H

#include <cstdint>
#include <vector>

#include <BDSP/streams/types.h>

#include "show.h"

bool is_equals(const uint8_t *data1, size_t size1, const uint8_t *data2, size_t size2);

bool is_equals(const uint8_t *data1, size_t size1, std::vector<uint8_t> data2);

bool is_equals(std::vector<uint8_t> data1, std::vector<uint8_t> data2);

void check_data_for_correctness(std::vector<uint8_t> &current,
                                std::vector<uint8_t> &correct,
                                bool use_hex_when_show = false);

void start_test_writer(BDSP::streams::IStreamWriter &writer,
                       std::vector<uint8_t> &current,
                       std::vector<uint8_t> &correct,
                       bool use_hex_when_show = false);

void start_test_reader(BDSP::streams::IStreamReader &reader,
                       std::vector<uint8_t> &current,
                       std::vector<uint8_t> &correct,
                       bool use_hex_when_show = false);

#endif // BDSP_TESTS_UTILS_TESTING_H
