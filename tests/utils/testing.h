#ifndef BDSP_TESTS_UTILS_TESTING_H
#define BDSP_TESTS_UTILS_TESTING_H

#include <cstdint>
#include <vector>

bool is_equals(const uint8_t *data1, size_t size1, const uint8_t *data2, size_t size2);

bool is_equals(const uint8_t *data1, size_t size1, std::vector<uint8_t> data2);

bool is_equals(std::vector<uint8_t> data1, std::vector<uint8_t> data2);

#endif //BDSP_TESTS_UTILS_TESTING_H
