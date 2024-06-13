#include "bdsp_mixins.h"

bdsp_set_max_packet_size_status BDSPMaxPacketSizeMixin::set_max_packet_size(uint16_t max_size) {
    if (max_packet_size < 1) return BDSP::INVALID_MAXIMUM_PACKET_SIZE;
    max_packet_size = max_size;
    return BDSP::MAX_PACKET_SIZE_SET;
}
