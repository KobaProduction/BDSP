#include "BDSP/checksums/crc8.h"

using namespace BDSP;
using namespace BDSP::checksums;

uint8_t core::crc8(const uint8_t *data_ptr, size_t size, uint8_t crc, uint8_t poly) {
    size_t i, j;
    for (i = 0; i < size; i++) {
        crc ^= data_ptr[i];
        for (j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = uint8_t(((crc << 1) ^ poly));
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint8_t checksums::crc8_cdma2000(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    return core::crc8(data_ptr, size, crc, 0x9B);
}

uint8_t checksums::crc8_dvb_s2(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    return core::crc8(data_ptr, size, crc, 0xD5);
}

uint8_t checksums::crc8_gsm_a(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    return core::crc8(data_ptr, size, crc, 0x1D);
}

uint8_t checksums::crc8_hitag(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    return core::crc8(data_ptr, size, crc, 0x1D);
}

uint8_t checksums::crc8_i_code(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    return core::crc8(data_ptr, size, crc, 0x1D);
}

uint8_t checksums::crc8_lte(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    return core::crc8(data_ptr, size, crc, 0x9B);
}

uint8_t checksums::crc8_mifare_mad(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    return core::crc8(data_ptr, size, crc, 0x1D);
}

uint8_t checksums::crc8_nrsc5(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    return core::crc8(data_ptr, size, crc, 0x31);
}

uint8_t checksums::crc8_opensafety(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    return core::crc8(data_ptr, size, crc, 0x2F);
}

uint8_t checksums::crc8_smbus(const uint8_t *data_ptr, size_t size, uint8_t crc) {
    return core::crc8(data_ptr, size, crc, 0x07);
}