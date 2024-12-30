#include <iostream>
#include "utils.h"

void show_data(uint8_t *data, size_t size, bool use_hex) {
    std::cout << "Data: ";
    for (int i = 0; i < size; ++i) {
        auto byte = static_cast<unsigned int>(data[i]);
        if (use_hex) {
            if (byte <= 0xF) std::cout << 0;
            std::cout << std::hex << std::uppercase;
        }
        std::cout << byte << " ";
        std::cout << std::dec;
    }
    std::cout << std::endl;
}

void show_data(std::vector<uint8_t> &data, bool use_hex) {
    show_data(data.data(), data.size(), use_hex);
}
