#ifndef BDSP_TRANSCEIVER_H
#define BDSP_TRANSCEIVER_H

#include "BDSP/receiver.h"
#include "BDSP/transmitter.h"

namespace BDSP {

template<typename TReadStream, typename TWriteStream>
class BDSPTransceiver final: public core::BDSPV1ReceiverCore, public core::BDSPV1TransmitterCore {
private:
    using core::BDSPV1ReceiverCore::set_stream_reader;
    using core::BDSPV1TransmitterCore::set_stream_writer;

public:
    BDSPTransceiver() {
        _reader = static_cast<streams::IStreamReader *>(new TReadStream());
        set_stream_reader(_reader);
        _writer = static_cast<streams::IStreamWriter *>(new TWriteStream());
        set_stream_writer(_writer);
    }

    ~BDSPTransceiver() {
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