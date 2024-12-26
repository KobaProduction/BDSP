#ifndef BDSP_RECEIVER_H
#define BDSP_RECEIVER_H

#include <BDSP/types.h>
#include "mixins.h"
#include <BDSP/packet.h>
#include <BDSP/encoders/cobs/decoder.h>
#include <BDSP/checksums/crc/crc8.h>

namespace BDSP {
    class BDSPReceiver : public BDSP::core::MaxPacketSizeMixin {
    public:
        BDSPReceiver();

        ~BDSPReceiver();

        set_config_status_t
        set_config(COBS::config_t cobs_config, packet_handler_t packet_handler, void *context = nullptr);

        void set_error_handler(receiver_error_handler_t error_handler, void *error_handler_context_ptr = nullptr);

        status_t parse(uint8_t *buffer_ptr, size_t size);

        status_t parse(uint8_t &byte);

    protected:
        void _parse_packet_byte(uint8_t byte, COBS::decode_state_t decode_state);

        void _reset();

        COBSDecoder *_decoder;
        packet_handler_t _packet_handler = nullptr;
        void *_packet_handler_context = nullptr;
        receiver_error_handler_t _error_handler = nullptr;
        void *_error_handler_context = nullptr;
        Packet *_raw_packet = nullptr;
        receiver_fsm_state_t _fsm_state = PACKET_ID;
        size_t _byte_received = 0;
        uint8_t _packet_checksum = 0;
    };
}


#endif //BDSP_RECEIVER_H