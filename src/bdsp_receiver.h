#ifndef BDSP_BDSP_RECEIVER_H
#define BDSP_BDSP_RECEIVER_H

#include "bdsp.h"
#include "bdsp_mixins.h"
#include "packet.h"
#include "encoders/cobs/decoder.h"
#include "checksums/crc/crc8.h"

using namespace BDSP;


class BDSPReceiver : public BDSPMaxPacketSizeMixin {
public:
    BDSPReceiver();
    ~BDSPReceiver();
    bdsp_set_config_status_t set_config(cobs_config_t cobs_config, packet_handler_t packet_handler, void *packet_handler_context_ptr = nullptr);
    bdsp_status_t parse(uint8_t *buffer_ptr, size_t size);
    bdsp_status_t parse(uint8_t &byte);
protected:
    void _parse_packet_byte(uint8_t byte, decode_state_t decode_state);
    void _reset();
    COBSDecoder *_decoder;
    packet_handler_t _packet_handler = nullptr;
    void *_packet_handler_context = nullptr;
    Packet *_raw_packet = nullptr;
    bdsp_receiver_fsm_state_t _fsm_state = PACKET_ID;
    size_t _byte_received = 0;
    uint8_t _packet_checksum = 0;
};

#endif //BDSP_BDSP_RECEIVER_H