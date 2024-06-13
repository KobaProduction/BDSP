#ifndef BDSP_BDSP_H
#define BDSP_BDSP_H

#ifdef ARDUINO
#include "stdint.h"
#include "stddef.h"
#else
#include <cstdint>
#include <cstddef>
#endif

#include "packet.h"
#include "encoders/cobs/encoder.h"
#include "encoders/cobs/decoder.h"
#include "checksums/crc/crc8.h"

#define DEFAULT_MAX_PACKET_SIZE 65535


namespace BDSP {
    enum bdsp_set_config_status {
        CONFIG_INSTALLED,
        CONFIG_ALREADY_INSTALLED,
        COBS_BUFFER_NOT_CREATED
    };

    enum bdsp_set_max_packet_size_status {
        MAX_PACKET_SIZE_SET,
        INVALID_MAXIMUM_PACKET_SIZE
    };

    enum bdsp_transmit_status {
        WRITE_OK,
        BDSP_CONFIG_NOT_INSTALLED,
        BDSP_EXCESS_SIZE_PACKET, // Data packet size exceeded.
    };

    typedef void (*packet_handler_t)(Packet &packet);
}

#endif //BDSP_BDSP_H
