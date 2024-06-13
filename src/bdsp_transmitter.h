#ifndef BDSP_BDSP_TRANSMITTER_H
#define BDSP_BDSP_TRANSMITTER_H

#include "BDSP.h"
#include "bdsp_mixins.h"
#include "encoders/cobs/encoder.h"
#include "packet.h"

using namespace BDSP;


class BDSPTransmitter : public BDSPMaxPacketSizeMixin {
public:
    BDSPTransmitter();
    ~BDSPTransmitter();
    bdsp_set_config_status  set_config(cobs_config_t config, write_handler_t write_handler);
    bdsp_transmit_status    send_data(uint8_t id, uint8_t *data, size_t size);
    bdsp_transmit_status    send_packet(Packet &packet);
private:
    COBSEncoder *encoder;
};

#endif //BDSP_BDSP_TRANSMITTER_H