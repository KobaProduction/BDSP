#ifndef BDSP_MIXINS_H
#define BDSP_MIXINS_H

#include "BDSP/checksums/crc8.h"
#include "BDSP/types.h"

#define DEFAULT_BDSP_MAX_PACKET_SIZE 65535

namespace BDSP::core {

enum set_max_packet_size_status_t { MAX_PACKET_SIZE_SET, INVALID_MAXIMUM_PACKET_SIZE };

class MaxPacketSizeMixin {
public:
    set_max_packet_size_status_t set_max_packet_size(uint16_t max_packet_size);

protected:
    uint16_t _max_packet_size = DEFAULT_BDSP_MAX_PACKET_SIZE;
};

typedef uint8_t (*checksum_function_t)(const uint8_t *data_ptr, size_t size, uint8_t base);

enum set_checksum_function_status_t { SET_CHECKSUM_FUNCTION_OK, CHECKSUM_FUNCTION_NULL_POINTER_ERROR };

class BDSPV1ChecksumMixin {
    checksum_function_t _checksum_function = BDSP::checksums::crc8_nrsc5;

public:
    set_checksum_function_status_t set_checksum_function(checksum_function_t _checksum_function);

protected:
    uint8_t _calc_checksum(bdsp_packet_v1_header header, uint8_t *data_ptr, uint16_t size) noexcept;
};

} // namespace BDSP::core

#endif // BDSP_MIXINS_H
