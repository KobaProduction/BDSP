#ifndef BDSP_CHECKSUMS_CRC_CRC8_H
#define BDSP_CHECKSUMS_CRC_CRC8_H

#include <stdint.h>
#include <stdlib.h>

namespace BDSP::utils {
    /**
     * Varieties of checksum function CRC-8/NRSC-5
     *
     * @param data_ptr a pointer to the data for which the calculation will be carried out..
     * @param size number of bytes of data.
     * @param base is used to continue the calculation when it occurs in parts. Submit to Base the past value of the control amount..
     * @return crs8 sum value.
     */
    uint8_t crc8_nrsc5(const uint8_t *data_ptr, size_t size, uint8_t base = 0xff);
}
#endif //BDSP_CHECKSUMS_CRC_CRC8_H
