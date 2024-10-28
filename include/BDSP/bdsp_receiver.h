#ifndef BDSP_BDSP_RECEIVER_H
#define BDSP_BDSP_RECEIVER_H

#include "BDSP/bdsp_types.h"
#include "BDSP/bdsp_mixins.h"
#include "BDSP/packet.h"
#include "BDSP/encoders/cobs/decoder.h"
#include "BDSP/checksums/crc/crc8.h"


class BDSPReceiver : public BDSPMaxPacketSizeMixin {
public:
    BDSPReceiver();
    ~BDSPReceiver();
    BDSP::set_config_status_t set_config(COBS::config_t cobs_config, BDSP::packet_handler_t packet_handler, void *packet_handler_context_ptr = nullptr);
    void set_error_handler(BDSP::receiver_error_handler_t error_handler, void *error_handler_context_ptr = nullptr);
    BDSP::status_t parse(uint8_t *buffer_ptr, size_t size);
    BDSP::status_t parse(uint8_t &byte);
protected:
    void _parse_packet_byte(uint8_t byte, COBS::decode_state_t decode_state);
    void _reset();
    COBSDecoder *_decoder;
    BDSP::packet_handler_t _packet_handler = nullptr;
    void *_packet_handler_context = nullptr;
    BDSP::receiver_error_handler_t _error_handler = nullptr;
    void *_error_handler_context = nullptr;
    Packet *_raw_packet = nullptr;
    BDSP::receiver_fsm_state_t _fsm_state = BDSP::PACKET_ID;
    size_t _byte_received = 0;
    uint8_t _packet_checksum = 0;
};

#endif //BDSP_BDSP_RECEIVER_H