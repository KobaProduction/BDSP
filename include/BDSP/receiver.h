#ifndef BDSP_RECEIVER_H
#define BDSP_RECEIVER_H

#include "BDSP/mixins.h"
#include "BDSP/streams/types.h"
#include "BDSP/types.h"

#include <stdlib.h>

namespace BDSP {

namespace core {

class ReceiverV1Core: public IReceiver, public core::MaxPacketSizeMixin, public core::BDSPV1ChecksumMixin {
protected:
    typedef enum {
        PACKET_HEADER,
        PACKET_SIZE_A,
        PACKET_SIZE_B,
        PACKET_DATA,
        PACKET_CHECKSUM,
        WAIT_END
    } receiver_fsm_state_t;

    receiver_fsm_state_t _fsm_state = PACKET_HEADER;
    receiver_error_handler_t _error_handler = nullptr;
    void *_error_handler_context = nullptr;
    core::packet_v1_header _packet_header{};
    packet_context_t _packet_context;

    packet_handler_t _packet_handler = nullptr;
    void *_packet_handler_context = nullptr;
    streams::IStreamReader *_reader = nullptr;
    uint16_t _received_packet_data_bytes = 0;

    void *(*_malloc)(size_t) = malloc;
    void (*_free)(void *) = free;

    void _deallocate_packet_memory();
    parse_packet_status_t _cause_error(parse_packet_status_t status);

public:
    ~ReceiverV1Core() override;
    parse_packet_status_t parse_packet_byte(uint8_t byte, streams::stream_read_status_t stream_status) override;
    void reset(bool need_wait_delimiter) override;
    void set_error_handler(receiver_error_handler_t error_handler, void *context) override;
    void set_packet_handler(packet_handler_t packet_handler, void *context) override;
    bdsp_set_stream_reader_status_t set_stream_reader(streams::IStreamReader *reader_ptr);
};

} // namespace core

template<typename TReadStream>
class Receiver: public core::ReceiverV1Core {
private:
    using core::ReceiverV1Core::set_stream_reader;

public:
    Receiver() {
        _reader = static_cast<streams::IStreamReader *>(new TReadStream());
        set_stream_reader(_reader);
    }

    ~Receiver() override {
        delete reinterpret_cast<TReadStream *>(_reader);
        _reader = nullptr;
    }

    TReadStream &get_read_stream() { return *reinterpret_cast<TReadStream *>(_reader); };
};

} // namespace BDSP

#endif // BDSP_RECEIVER_H