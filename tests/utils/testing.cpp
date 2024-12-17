#include "testing.h"

bool is_equals(const uint8_t *data1, size_t size1, const uint8_t *data2, size_t size2) {
    if (size1 not_eq size2) return false;
    bool equals = true;
    for (size_t i = 0; i < size1; ++i) {
        if (data1[i] == data2[i]) continue;
        equals = false;
        break;
    }
    return equals;
}

bool is_equals(const uint8_t *data1, size_t size1, std::vector<uint8_t> data2) {
    return is_equals(data1, size1, data2.data(), data2.size());
}

bool is_equals(std::vector<uint8_t> data1, std::vector<uint8_t> data2) {
    return is_equals(data1.data(), data1.size(), data2.data(), data2.size());
}
