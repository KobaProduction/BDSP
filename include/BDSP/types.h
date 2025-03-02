#ifndef BDSP_TYPES_H
#define BDSP_TYPES_H

#include <stdint.h>

#include <BDSP/streams/types.h>

namespace BDSP {

namespace core {

struct packet_v1_header {
    uint8_t packet_id : 5;
    bool is_two_bytes_for_packet_size : 1;
    bool is_checksum_used : 1;
    bool is_unsupported_protocol_version : 1;
};

} // namespace core

struct packet_context_t {
    uint8_t packet_id{};
    uint8_t *data_ptr = nullptr;
    uint16_t size = 0;
    bool need_clear = false;
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
    STREAM_READING_ERROR,
    EXCEEDING_THE_MAXIMUM_PACKET_SIZE_ERROR,
    NOT_ENOUGH_RAM_FOR_PACKET_ERROR,
    CHECKSUM_ERROR,
    UNSUPPORTED_PROTOCOL_ERROR,
    UNKNOWN_READER_ERROR
};

typedef void (*receiver_error_handler_t)(parse_packet_status_t error, void *packet_handler_context);

typedef void (*packet_handler_t)(packet_context_t &packet_context, void *packet_handler_context);

enum bdsp_set_stream_reader_status_t {
    SET_STREAM_READER_OK,
    RESET_STREAM_READER,
    CHANGE_STREAM_READER,
    STREAM_READER_NULL_POINTER_ERROR,
    STREAM_READER_ALREADY_USED_ERROR
};

enum set_stream_writer_status_t { SET_STREAM_WRITER_OK, RESET_STREAM_WRITER, STREAM_WRITER_NOT_READY };

class IReceiver {
public:
    virtual parse_packet_status_t parse_packet_byte(uint8_t byte, streams::stream_read_status_t stream_status) = 0;
    virtual void reset(bool need_wait_delimiter) = 0;
    virtual void set_error_handler(receiver_error_handler_t error_handler, void *context) = 0;
    virtual void set_packet_handler(packet_handler_t packet_handler, void *context) = 0;
    virtual ~IReceiver() = default;
};

class ITransmitter {
public:
    virtual send_packet_status_t send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size) = 0;
    virtual send_packet_status_t send_data(uint8_t packet_id,
                                           uint8_t *buffer_ptr,
                                           size_t size,
                                           bool checksum_enforcement) = 0;
    virtual send_packet_status_t send_packet(packet_context_t &packet_context) = 0;
    virtual send_packet_status_t send_packet(packet_context_t &packet_context, bool checksum_enforcement) = 0;
    virtual void set_checksum_usage_default_state(bool using_checksum) = 0;
    virtual ~ITransmitter() = default;
};

} // namespace BDSP

#endif // BDSP_TYPES_H
