#ifndef BDSP_TYPES_H
#define BDSP_TYPES_H

#include <stdint.h>

namespace BDSP {

namespace core {

struct packet_v1_header {
    uint8_t packet_id : 4;
    bool is_service_packet : 1;
    bool is_two_bytes_for_packet_size : 1;
    bool is_checksum_used : 1;
    bool is_unsupported_protocol_version : 1;
};

} // namespace core

struct packet_context_t {
    uint8_t packet_id{};
    uint8_t *data_ptr = nullptr;
    uint16_t size = 0;
    bool need_clear{};
};

enum send_packet_status_t {
    SEND_PACKET_OK,
    STREAM_WRITER_NOT_SET_ERROR,
    MAXIMUM_PACKET_SIZE_EXCEEDING_ERROR,
    PACKET_ID_ERROR
};

enum parse_packet_status_t {
    PARSE_PACKET_BYTE_OK,
    PACKET_HANDLER_NOT_SET_ERROR,
    SERVICE_PACKET_HANDLER_NOT_SET_ERROR,
    STREAM_READING_ERROR,
    EXCEEDING_THE_MAXIMUM_PACKET_SIZE_ERROR,
    NOT_ENOUGH_RAM_FOR_PACKET_ERROR,
    CHECKSUM_ERROR,
    UNSUPPORTED_PROTOCOL_ERROR
};

typedef void (*receiver_error_handler_t)(parse_packet_status_t error, void *packet_handler_context);

typedef void (*packet_handler_t)(packet_context_t &packet_context, void *packet_handler_context);
} // namespace BDSP

#endif // BDSP_TYPES_H
