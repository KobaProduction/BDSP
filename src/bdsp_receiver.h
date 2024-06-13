#ifndef BDSP_BDSP_RECEIVER_H
#define BDSP_BDSP_RECEIVER_H

#include "BDSP.h"
#include "bdsp_mixins.h"
#include "packet.h"
#include "encoders/cobs/decoder.h"
#include "checksums/crc/crc8.h"

enum bdsp_receiver_fsm_state {PACKET_ID, SIZE_A, SIZE_B, PACKET_DATA, PACKET_CHECKSUM, WAIT_NEW_PACKET};

class BDSPReceiver : public BDSPMaxPacketSizeMixin {
public:
    BDSPReceiver();
    ~BDSPReceiver();
    bdsp_set_config_status set_config(cobs_config_t config, packet_handler_t packet_handler);
    void parse(uint8_t *data_ptr, size_t size);
private:
    void parse_packet_byte(uint8_t character, cobs_read_state read_state);
    void reset();
    COBSDecoder *decoder;
    packet_handler_t packet_handler = nullptr;
    Packet *raw_packet;
    bdsp_receiver_fsm_state parse_state = PACKET_ID;
    size_t received_data_byte = 0;
    uint8_t packet_checksum = 0;
};

#endif //BDSP_BDSP_RECEIVER_H