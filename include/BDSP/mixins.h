#ifndef BDSP_MIXINS_H
#define BDSP_MIXINS_H

#include "BDSP/types.h"

#define DEFAULT_BDSP_MAX_PACKET_SIZE 65535

namespace BDSP::core {

    enum set_max_packet_size_status_t {
        MAX_PACKET_SIZE_SET,
        INVALID_MAXIMUM_PACKET_SIZE
    };

    class MaxPacketSizeMixin {
    public:
        set_max_packet_size_status_t set_max_packet_size(uint16_t max_packet_size);
    protected:
        uint16_t _max_packet_size = DEFAULT_BDSP_MAX_PACKET_SIZE;
    };

}


#endif //BDSP_MIXINS_H
