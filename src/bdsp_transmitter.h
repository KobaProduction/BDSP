#ifndef BDSP_BDSP_TRANSMITTER_H
#define BDSP_BDSP_TRANSMITTER_H

#include "bdsp_types.h"
#include "bdsp_mixins.h"
#include "packet.h"
#include "encoders/cobs/encoder.h"
#include "checksums/crc/crc8.h"


class BDSPTransmitter : public BDSPMaxPacketSizeMixin {
public:
    BDSPTransmitter();
    ~BDSPTransmitter();
    BDSP::set_config_status_t set_config(COBS::config_t cobs_config, COBS::write_handler_t write_handler, void *write_handler_context);
    BDSP::status_t   send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size);
    BDSP::status_t   send_packet(Packet &packet);
protected:
    COBSEncoder *_encoder;
};

#endif //BDSP_BDSP_TRANSMITTER_H