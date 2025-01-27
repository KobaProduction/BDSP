#include "BDSP/checksums/crc8.h"

using namespace BDSP;
using namespace BDSP::checksums;

uint8_t checksums::crc8_nrsc5(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    size_t i, j;
    for (i = 0; i < size; i++) {
        crc ^= data_ptr[i];
        for (j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = uint8_t(((crc << 1) ^ 0x31));
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint8_t checksums::crc8_lte(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    size_t i, j;
    for (i = 0; i < size; i++) {
        crc ^= data_ptr[i];
        for (j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = uint8_t(((crc << 1) ^ 0x9B));
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}
