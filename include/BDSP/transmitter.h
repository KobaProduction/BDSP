#ifndef BDSP_TRANSMITTER_H
#define BDSP_TRANSMITTER_H

#include "BDSP/types.h"
#include "BDSP/mixins.h"
#include "BDSP/streams/types.h"

namespace BDSP {

    class BDSPTransmitter : public core::MaxPacketSizeMixin {
    public:
        // todo rename to set_stream_writer
        void set_writer(streams::IWriter *writer_ptr);

        status_t send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size);

        status_t send_packet(uint8_t packet_id, bdsp_packet_t &packet, bool use_crc = true);

        // todo transaction
    protected:
        streams::IWriter *_writer = nullptr;
    };
}


#endif //BDSP_TRANSMITTER_H