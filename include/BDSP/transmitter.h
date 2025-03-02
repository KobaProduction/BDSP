#ifndef BDSP_TRANSMITTER_H
#define BDSP_TRANSMITTER_H

#include "BDSP/mixins.h"
#include "BDSP/streams/types.h"
#include "BDSP/types.h"

namespace BDSP {

namespace core {
class BDSPV1TransmitterCore: public ITransmitter, public core::MaxPacketSizeMixin, public core::BDSPV1ChecksumMixin {
protected:
    bool _checksum_usage_default_state = true;
    streams::IStreamWriter *_writer = nullptr;

public:
    send_packet_status_t send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size) override;
    send_packet_status_t send_data(uint8_t packet_id,
                                   uint8_t *buffer_ptr,
                                   size_t size,
                                   bool checksum_enforcement) override;
    send_packet_status_t send_packet(packet_context_t &packet_context) noexcept override;
    send_packet_status_t send_packet(packet_context_t &packet_context, bool checksum_enforcement) noexcept override;
    void set_checksum_usage_default_state(bool using_checksum) override;
    set_stream_writer_status_t set_stream_writer(streams::IStreamWriter *writer_ptr);
};
} // namespace core

template<typename TWriteStream>
class BDSPTransmitter: public core::BDSPV1TransmitterCore {
private:
    using core::BDSPV1TransmitterCore::set_stream_writer;

public:
    BDSPTransmitter() {
        _writer = static_cast<streams::IStreamWriter *>(new TWriteStream());
        set_stream_writer(_writer);
    }

    ~BDSPTransmitter() override {
        delete reinterpret_cast<TWriteStream *>(_writer);
        _writer = nullptr;
    }

    TWriteStream &get_write_stream() { return *reinterpret_cast<TWriteStream *>(_writer); };
};

} // namespace BDSP

#endif // BDSP_TRANSMITTER_H