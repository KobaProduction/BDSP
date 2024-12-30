#ifndef BDSP_RECEIVER_H
#define BDSP_RECEIVER_H

#include <BDSP/types.h>
#include <BDSP/mixins.h>
#include <BDSP/packet.h>
#include <BDSP/streams/types.h>
#include <BDSP/checksums/crc/crc8.h>

namespace BDSP {
    class BDSPReceiver : public core::MaxPacketSizeMixin {
    public:
        BDSPReceiver();

        ~BDSPReceiver();

        void set_decoder(streams::IDecoder *decoder_ptr);
        void set_packet_handler(packet_handler_t packet_handler, void *context = nullptr);

        void set_error_handler(receiver_error_handler_t error_handler, void *error_handler_context_ptr = nullptr);

        status_t parse(uint8_t *buffer_ptr, size_t size);

        status_t parse(uint8_t &byte);

    protected:
        void _parse_packet_byte(uint8_t byte, streams::decode_status_t decode_status);

        void _reset();

        streams::IDecoder *_decoder = nullptr;
        packet_handler_t _packet_handler = nullptr;
        void *_packet_handler_context = nullptr;
        receiver_error_handler_t _error_handler = nullptr;
        void *_error_handler_context = nullptr;
        Packet *_raw_packet = nullptr;
        core::receiver_fsm_state_t _fsm_state = core::PACKET_ID;
        size_t _byte_received = 0;
        uint8_t _packet_checksum = 0;
    };
}


#endif //BDSP_RECEIVER_H