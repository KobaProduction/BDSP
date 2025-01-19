#ifndef BDSP_TYPES_H
#define BDSP_TYPES_H

#include "BDSP/packet.h"

namespace BDSP {

    enum status_t {
        WRITE_OK,
        PARSE_OK,
        BDSP_CONFIG_NOT_INSTALLED,
        BDSP_EXCESS_SIZE_PACKET, // Data packet size exceeded.
    };

    namespace core {
        enum receiver_fsm_state_t {PACKET_ID, SIZE_A, SIZE_B, PACKET_DATA, PACKET_CHECKSUM, WAIT_END};
    }

    enum receiver_error_t {
        ERROR_DECODING = 1,
        EXCEEDING_THE_MAXIMUM_PACKET_SIZE,
        NOT_ENOUGH_RAM_FOR_PACKET,
        PACKET_CHECKSUM_DOES_NOT_MATCH
    };

    typedef void (*receiver_error_handler_t)(receiver_error_t error, void *packet_handler_context);

    typedef void (*packet_handler_t)(Packet &packet, void *packet_handler_context);
}

#endif //BDSP_TYPES_H
