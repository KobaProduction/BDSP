#ifndef BDSP_BDSP_MIXINS_H
#define BDSP_BDSP_MIXINS_H

#include "BDSP.h"

using namespace BDSP;

class BDSPMaxPacketSizeMixin {
public:
    bdsp_set_max_packet_size_status set_max_packet_size(uint16_t max_size);
protected:
    uint16_t max_packet_size = DEFAULT_MAX_PACKET_SIZE;
};

#endif //BDSP_BDSP_MIXINS_H
