#include "./crc8.h"

uint8_t crc8(const uint8_t *data_ptr, size_t size, uint8_t base) {
    uint8_t crc = base;
    size_t i, j;
    for (i = 0; i < size; i++) {
        crc ^= data_ptr[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0) {
                crc = uint8_t(((crc << 1) ^ 0x31));
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}