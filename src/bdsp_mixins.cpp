#include "bdsp_mixins.h"

bdsp_set_max_packet_size_status_t BDSPMaxPacketSizeMixin::set_max_packet_size(uint16_t max_packet_size) {
    if (_max_packet_size < 1) return BDSP::INVALID_MAXIMUM_PACKET_SIZE;
    _max_packet_size = max_packet_size;
    return BDSP::MAX_PACKET_SIZE_SET;
}