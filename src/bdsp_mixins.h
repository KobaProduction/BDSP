#ifndef BDSP_BDSP_MIXINS_H
#define BDSP_BDSP_MIXINS_H

#include "bdsp.h"

using namespace BDSP;


class BDSPMaxPacketSizeMixin {
public:
    bdsp_set_max_packet_size_status_t set_max_packet_size(uint16_t max_packet_size);
protected:
    uint16_t _max_packet_size = DEFAULT_MAX_PACKET_SIZE;
};

#endif //BDSP_BDSP_MIXINS_H
