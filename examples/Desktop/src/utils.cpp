#include <iostream>
#include "utils.h"

void show_data(std::vector<uint8_t> &data, bool use_hex) {
    std::cout << "Data: ";
    for (int i = 0; i < data.size(); ++i) {
        auto byte = static_cast<unsigned int>(data[i]);
        if (use_hex) std::cout << std::hex << std::uppercase;
        std::cout << byte << " ";
    }
    std::cout << std::endl;
}
