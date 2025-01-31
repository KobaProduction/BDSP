#ifndef BDSP_TRANSMITTER_H
#define BDSP_TRANSMITTER_H

#include "BDSP/mixins.h"
#include "BDSP/streams/types.h"
#include "BDSP/types.h"

namespace BDSP {



enum checksum_usage_state_t { DEFAULT_CHECKSUM, WITHOUT_CHECKSUM, WITH_CHECKSUM };

namespace core {
class BDSPV1TransmitterCore: public core::MaxPacketSizeMixin, public core::BDSPV1ChecksumMixin {
protected:
    streams::IWriter *_writer = nullptr;
    bool _checksum_usage_default_state = true;

public:
    bdsp_transmitter_send_packet_status_t send_data(uint8_t packet_id,
                                                    uint8_t *buffer_ptr,
                                                    size_t size,
                                                    checksum_usage_state_t checksum_state = DEFAULT_CHECKSUM);
    bdsp_transmitter_send_packet_status_t send_packet(bdsp_packet_context_t &packet_context,
                                                      checksum_usage_state_t checksum_state = DEFAULT_CHECKSUM);
    void set_checksum_usage_default_state(bool using_checksum);
    void set_stream_writer(streams::IWriter *writer_ptr);
};
} // namespace core

class BDSPTransmitter final: public core::BDSPV1TransmitterCore { };

} // namespace BDSP

#endif // BDSP_TRANSMITTER_H