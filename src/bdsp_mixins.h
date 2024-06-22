#ifndef BDSP_BDSP_MIXINS_H
#define BDSP_BDSP_MIXINS_H

#include "bdsp_types.h"

#define DEFAULT_BDSP_MAX_PACKET_SIZE 65535

class BDSPMaxPacketSizeMixin {
public:
    BDSP::set_max_packet_size_status_t set_max_packet_size(uint16_t max_packet_size);
protected:
    uint16_t _max_packet_size = DEFAULT_BDSP_MAX_PACKET_SIZE;
};

#endif //BDSP_BDSP_MIXINS_H
