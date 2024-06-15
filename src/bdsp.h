#ifndef BDSP_BDSP_H
#define BDSP_BDSP_H

#include "stdint.h"
#include "stddef.h"

#include "packet.h"
#include "encoders/cobs/encoder.h"
#include "encoders/cobs/decoder.h"
#include "checksums/crc/crc8.h"

#define DEFAULT_MAX_PACKET_SIZE 65535


namespace BDSP {

    enum bdsp_set_max_packet_size_status_t {
        MAX_PACKET_SIZE_SET,
        INVALID_MAXIMUM_PACKET_SIZE
    };

    enum bdsp_set_config_status_t {
        CONFIG_INSTALLED,
        CONFIG_ALREADY_INSTALLED,
        COBS_BUFFER_NOT_CREATED
    };

    enum bdsp_status_t {
        WRITE_OK,
        PARSE_OK,
        BDSP_CONFIG_NOT_INSTALLED,
        BDSP_EXCESS_SIZE_PACKET, // Data packet size exceeded.
    };

    enum bdsp_receiver_fsm_state_t {PACKET_ID, SIZE_A, SIZE_B, PACKET_DATA, PACKET_CHECKSUM, WAIT_END};

    typedef void (*packet_handler_t)(Packet &packet, void *packet_handler_context);
}

#endif //BDSP_BDSP_H
