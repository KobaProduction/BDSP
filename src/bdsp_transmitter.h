#ifndef BDSP_BDSP_TRANSMITTER_H
#define BDSP_BDSP_TRANSMITTER_H

#include "bdsp.h"
#include "bdsp_mixins.h"
#include "encoders/cobs/encoder.h"
#include "packet.h"

using namespace BDSP;


class BDSPTransmitter : public BDSPMaxPacketSizeMixin {
public:
    BDSPTransmitter();
    ~BDSPTransmitter();
    bdsp_set_config_status_t set_config(cobs_config_t cobs_config, cobs_write_handler_t write_handler, void *write_handler_context);
    bdsp_status_t   send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size);
    bdsp_status_t   send_packet(Packet &packet);
protected:
    COBSEncoder *_encoder;
};

#endif //BDSP_BDSP_TRANSMITTER_H