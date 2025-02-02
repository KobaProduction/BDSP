#include "BDSP/mixins.h"

using namespace BDSP::core;

set_max_packet_size_status_t MaxPacketSizeMixin::set_max_packet_size(uint16_t max_packet_size) {
    if (max_packet_size < 1) {
        return INVALID_MAXIMUM_PACKET_SIZE;
    }
    _max_packet_size = max_packet_size;
    return MAX_PACKET_SIZE_SET;
}

uint8_t BDSPV1ChecksumMixin::_calc_checksum(packet_v1_header header, uint8_t *data_ptr, uint16_t size) noexcept {
    if (not header.is_checksum_used) {
        return 0;
    }
    uint8_t checksum = _checksum_function(reinterpret_cast<uint8_t *>(&header), 1, 255);
    uint8_t size_byte = size;
    checksum = _checksum_function(&size_byte, 1, checksum);
    if (header.is_two_bytes_for_packet_size) {
        size_byte = size >> 8;
        checksum = _checksum_function(&size_byte, 1, checksum);
    }
    checksum = _checksum_function(data_ptr, size, checksum);
    return checksum;
}

set_checksum_function_status_t BDSPV1ChecksumMixin::set_checksum_function(checksum_function_t checksum_function) {
    if (checksum_function not_eq nullptr) {
        _checksum_function = checksum_function;
        return SET_CHECKSUM_FUNCTION_OK;
    }
    return CHECKSUM_FUNCTION_NULL_POINTER_ERROR;
}
