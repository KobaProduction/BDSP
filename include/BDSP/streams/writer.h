#ifndef BDSP_STREAMS_STREAM_WRITER_H
#define BDSP_STREAMS_STREAM_WRITER_H

#include "BDSP/streams/mixins.h"
#include "BDSP/streams/strategies/types.h"
#include "BDSP/streams/types.h"

namespace BDSP::streams {
namespace core {
class StreamWriterCore: public IStreamWriter, public core::StreamReadyMixin {
private:
    stream_write_handler_t _handler = nullptr;
    void *_handler_context = nullptr;

protected:
    strategies::IStreamWriteStrategy *_strategy = nullptr;

public:
    bool get_ready_status() final;
    stream_write_status_t finish() final;
    void set_strategy(strategies::IStreamWriteStrategy &strategy) noexcept;
    void set_stream_writer(stream_write_handler_t handler, void *context_ptr) final;
    stream_write_status_t write(uint8_t byte) final;
    stream_write_status_t write(uint8_t *buffer_ptr, size_t size) final;
};
} // namespace core

template<typename TStrategy>
class StreamWriter: public core::StreamWriterCore {
protected:
    using core::StreamWriterCore::set_strategy;

public:
    StreamWriter() {
        _strategy = static_cast<strategies::IStreamWriteStrategy *>(new TStrategy());
        set_strategy(*_strategy);
    }

    ~StreamWriter() { delete _strategy; }

    TStrategy &get_strategy() { return *reinterpret_cast<TStrategy *>(_strategy); };
};

} // namespace BDSP::streams

#endif // BDSP_STREAMS_STREAM_WRITER_H
