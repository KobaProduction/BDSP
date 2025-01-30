#ifndef BDSP_TYPES_H
#define BDSP_TYPES_H

#include <stdint.h>

namespace BDSP {

namespace core {

struct bdsp_packet_v1_header {
    bool is_unsupported_protocol_version : 1;
    bool is_two_bytes_for_packet_size : 1;
    bool is_checksum_used : 1;
    bool is_service_packet : 1;
    uint8_t packet_id : 4;
};

} // namespace core

struct bdsp_packet_context_t {
    uint8_t packet_id{};
    uint8_t *data_ptr = nullptr;
    uint16_t size = 0;
    bool need_clear{};
};

enum status_t {
    BDSP_WRITE_OK,
    BDSP_PARSE_OK,
    BDSP_CONFIG_NOT_INSTALLED,
    BDSP_EXCESS_SIZE_PACKET, // Data packet size exceeded.
    BDSP_ERROR_PACKET_ID
};

namespace core {
enum receiver_fsm_state_t { PACKET_HEADER, PACKET_SIZE_A, PACKET_SIZE_B, PACKET_DATA, PACKET_CHECKSUM, WAIT_END };
typedef void *(*bdsp_memory_allocator_t)(size_t);
typedef void (*bdsp_memory_cleaner_t)(void *);

} // namespace core

enum receiver_error_t {
    ERROR_STREAM_READING = 1,
    EXCEEDING_THE_MAXIMUM_PACKET_SIZE,
    NOT_ENOUGH_RAM_FOR_PACKET,
    CHECKSUM_ERROR,
    UNSUPPORTED_PROTOCOL
};

typedef void (*receiver_error_handler_t)(receiver_error_t error, void *packet_handler_context);

typedef void (*packet_handler_t)(bdsp_packet_context_t &packet_context, void *packet_handler_context);
} // namespace BDSP

#endif // BDSP_TYPES_H
