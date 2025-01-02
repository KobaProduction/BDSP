#ifndef BDSP_DESKTOP_UTILS_H
#define BDSP_DESKTOP_UTILS_H

#include <vector>
#include <cstdint>

void show_data_(uint8_t *data, size_t size, bool use_hex = false);

void show_data_(std::vector<uint8_t> &data, bool use_hex = false);

#endif //BDSP_DESKTOP_UTILS_H
