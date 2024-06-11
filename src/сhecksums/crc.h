#ifndef BDSP_CRC_H
#define BDSP_CRC_H

#include <stdint.h>
#include <stdlib.h>

uint8_t crc8(uint8_t *data_ptr, size_t len, uint8_t base = 0xff);

#endif //BDSP_CRC_H
