#ifndef BDSP_TRANSMITTER_H
#define BDSP_TRANSMITTER_H

#include <BDSP/types.h>
#include "mixins.h"
#include <BDSP/packet.h>
#include <BDSP/encoders/cobs/encoder.h>
#include <BDSP/checksums/crc/crc8.h>

namespace BDSP {

    class BDSPTransmitter : public BDSP::core::MaxPacketSizeMixin {
    public:
        BDSPTransmitter();

        ~BDSPTransmitter();

        set_config_status_t set_config(
            COBS::config_t cobs_config,
            COBS::write_handler_t write_handler,
            void *write_handler_context = nullptr
        );

        status_t send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size);

        status_t send_packet(Packet &packet);

    protected:
        COBSEncoder *_encoder;
    };
}


#endif //BDSP_TRANSMITTER_H