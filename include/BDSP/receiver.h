#ifndef BDSP_RECEIVER_H
#define BDSP_RECEIVER_H

#include "BDSP/mixins.h"
#include "BDSP/streams/types.h"
#include "BDSP/types.h"

namespace BDSP {

namespace core {

class BDSPV1Receiver: public core::MaxPacketSizeMixin, public core::BDSPV1ChecksumMixin {
public:
    BDSPV1Receiver();

    ~BDSPV1Receiver();

    void set_reader(streams::IReader *reader_ptr);
    void set_packet_handler(packet_handler_t packet_handler, void *context = nullptr);

    //        void set_error_handler(receiver_error_handler_t error_handler, void *error_handler_context_ptr = nullptr);

    status_t parse(uint8_t *buffer_ptr, size_t size);

    status_t parse(uint8_t &byte);

protected:
    void _handle_error(receiver_error_t error);

    void _parse_packet_byte(uint8_t byte, streams::read_status_t decode_status);

    void _reset();

    streams::IReader *_reader = nullptr;
    packet_handler_t _packet_handler = nullptr;
    void *_packet_handler_context = nullptr;
    receiver_error_handler_t _error_handler = nullptr;
    void *_error_handler_context = nullptr;

    core::bdsp_packet_v1_header _packet_header{};
    bdsp_packet_t _raw_packet;
    core::receiver_fsm_state_t _fsm_state = core::PACKET_HEADER;
    size_t _byte_received = 0;
};

} // namespace core

class BDSPReceiver final: public core::BDSPV1Receiver { };
} // namespace BDSP

#endif // BDSP_RECEIVER_H