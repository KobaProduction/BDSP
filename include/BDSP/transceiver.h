#ifndef BDSP_TRANSCEIVER_H
#define BDSP_TRANSCEIVER_H

#include "BDSP/receiver.h"
#include "BDSP/transmitter.h"

namespace BDSP {

template<typename TReadStream, typename TWriteStream>
class Transceiver: public core::ReceiverV1Core, public core::TransmitterV1Core {
private:
    using core::ReceiverV1Core::set_stream_reader;
    using core::TransmitterV1Core::set_stream_writer;

public:
    Transceiver() {
        _reader = static_cast<streams::IStreamReader *>(new TReadStream());
        set_stream_reader(_reader);
        _writer = static_cast<streams::IStreamWriter *>(new TWriteStream());
        set_stream_writer(_writer);
    }

    ~Transceiver() override {
        delete reinterpret_cast<TReadStream*>(_reader);
        _reader = nullptr;
        delete reinterpret_cast<TWriteStream*>(_writer);
        _writer = nullptr;
    }

    TReadStream &get_read_stream() { return *reinterpret_cast<TReadStream *>(_reader); };
    TWriteStream &get_write_stream() { return *reinterpret_cast<TWriteStream *>(_writer); };

};

} // namespace BDSP

#endif // BDSP_TRANSCEIVER_H