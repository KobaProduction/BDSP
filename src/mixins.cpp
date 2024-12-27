#include <BDSP/mixins.h>

using namespace BDSP::core;

set_max_packet_size_status_t MaxPacketSizeMixin::set_max_packet_size(uint16_t max_packet_size) {
    if (max_packet_size < 1) return INVALID_MAXIMUM_PACKET_SIZE;
    _max_packet_size = max_packet_size;
    return MAX_PACKET_SIZE_SET;
}
