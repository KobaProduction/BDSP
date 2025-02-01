#ifndef BDSP_RECEIVER_H
#define BDSP_RECEIVER_H

#include "BDSP/mixins.h"
#include "BDSP/streams/types.h"
#include "BDSP/types.h"

namespace BDSP {

enum bdsp_set_stream_reader_status_t {
    SET_STREAM_READER_OK,
    STREAM_READER_NULL_POINTER_ERROR,
    STREAM_READER_NOT_READY_ERROR
};

namespace core {

enum receiver_fsm_state_t { PACKET_HEADER, PACKET_SIZE_A, PACKET_SIZE_B, PACKET_DATA, PACKET_CHECKSUM, WAIT_END };

class BDSPV1Receiver: public core::MaxPacketSizeMixin, public core::BDSPV1ChecksumMixin {
protected:
    core::receiver_fsm_state_t _fsm_state = core::PACKET_HEADER;
    uint16_t _received_packet_data_bytes = 0;

    packet_handler_t _packet_handler = nullptr;
    void *_packet_handler_context = nullptr;

    packet_handler_t _service_packet_handler = nullptr;
    void *_service_packet_handler_context = nullptr;

    receiver_error_handler_t _error_handler = nullptr;
    void *_error_handler_context = nullptr;
    streams::IReader *_reader = nullptr;

    core::bdsp_packet_v1_header _packet_header{};
    bdsp_packet_context_t _packet_context;

    void _deallocate_packet_memory();
    bdsp_receiver_parse_status_t _cause_error(bdsp_receiver_parse_status_t status);

public:
    ~BDSPV1Receiver();
    bdsp_receiver_parse_status_t parse_packet_byte(uint8_t byte, streams::read_status_t stream_status);
    void reset(bool need_wait_delimiter = false);
    void set_error_handler(receiver_error_handler_t error_handler, void *context = nullptr);
    void set_packet_handler(packet_handler_t packet_handler, void *context = nullptr);
    void set_service_packet_handler(packet_handler_t service_packet_handler, void *context = nullptr);
    bdsp_set_stream_reader_status_t set_stream_reader(streams::IReader *reader_ptr);
};

} // namespace core

class BDSPReceiver final: public core::BDSPV1Receiver { };
} // namespace BDSP

#endif // BDSP_RECEIVER_H