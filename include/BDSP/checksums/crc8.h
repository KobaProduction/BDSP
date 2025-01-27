#ifndef BDSP_CHECKSUMS_CRC8_H
#define BDSP_CHECKSUMS_CRC8_H

#include <stdint.h>
#include <stdlib.h>

namespace BDSP::checksums {

namespace core {
/**
 * Checksum core function.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @param poly is crc polynomial.
 * @return crc8 result value.
 */
uint8_t crc8(const uint8_t *data_ptr, size_t size, uint8_t crc, uint8_t poly);
}

/**
 * Variant of checksum function CRC-8/CDMA2000. Init value - 0xFF, polynomial - 0x9B.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @return crc8 result value.
 */
uint8_t crc8_cdma2000(const uint8_t *data_ptr, size_t size, uint8_t crc = 0xFF);

/**
 * Variant of checksum function CRC-8/DVB-S2. Init value - 0x00, polynomial - 0xD5.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @return crc8 result value.
 */
uint8_t crc8_dvb_s2(const uint8_t *data_ptr, size_t size, uint8_t crc = 0x00);

/**
 * Variant of checksum function CRC-8/GSM-A. Init value - 0x00, polynomial - 0x1D.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @return crc8 result value.
 */
uint8_t crc8_gsm_a(const uint8_t *data_ptr, size_t size, uint8_t crc = 0x00);

/**
 * Variant of checksum function CRC-8/HITAG. Init value - 0xFF, polynomial - 0x1D.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @return crc8 result value.
 */
uint8_t crc8_hitag(const uint8_t *data_ptr, size_t size, uint8_t crc = 0xFF);

/**
 * Variant of checksum function CRC-8/I-CODE. Init value - 0xFD, polynomial - 0x1D.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @return crc8 result value.
 */
uint8_t crc8_i_code(const uint8_t *data_ptr, size_t size, uint8_t crc = 0xFD);

/**
 * Variant of checksum function CRC-8/LTE. Init value - 0x00, polynomial - 0x9B.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @return crc8 result value.
 */
uint8_t crc8_lte(const uint8_t *data_ptr, size_t size, uint8_t crc = 0x00);

/**
 * Variant of checksum function CRC-8/MIFARE-MAD. Init value - 0xC7, polynomial - 0x1D.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @return crc8 result value.
 */
uint8_t crc8_mifare_mad(const uint8_t *data_ptr, size_t size, uint8_t crc = 0xC7);

/**
 * Variant of checksum function CRC-8/NRSC-5. Init value - 0xFF, polynomial - 0x31.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @return crc8 result value.
 */
uint8_t crc8_nrsc5(const uint8_t *data_ptr, size_t size, uint8_t crc = 0xFF);

/**
 * Variant of checksum function CRC-8/OPENSAFETY. Init value - 0x00, polynomial - 0x2F.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @return crc8 result value.
 */
uint8_t crc8_opensafety(const uint8_t *data_ptr, size_t size, uint8_t crc = 0x00);

/**
 * Variant of checksum function CRC-8/SMBUS. Init value - 0x00, polynomial - 0x07.
 *
 * @param data_ptr a pointer to the data for which the calculation will be carried out.
 * @param size number of bytes of data.
 * @param crc is used to continue the calculation when it occurs in parts.
 * @return crc8 result value.
 */
uint8_t crc8_smbus(const uint8_t *data_ptr, size_t size, uint8_t crc = 0x00);
} // namespace BDSP::checksums

#endif // BDSP_CHECKSUMS_CRC8_H
