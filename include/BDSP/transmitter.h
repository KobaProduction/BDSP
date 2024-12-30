#ifndef BDSP_TRANSMITTER_H
#define BDSP_TRANSMITTER_H

#include <BDSP/types.h>
#include <BDSP/mixins.h>
#include <BDSP/packet.h>
#include <BDSP/streams/types.h>
#include <BDSP/checksums/crc/crc8.h>

namespace BDSP {

    class BDSPTransmitter : public core::MaxPacketSizeMixin {
    public:
        void set_encoder(streams::IEncoder *encoder_ptr);

        status_t send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size);

        status_t send_packet(Packet &packet);

    protected:
        streams::IEncoder *_encoder = nullptr;
    };
}


#endif //BDSP_TRANSMITTER_H