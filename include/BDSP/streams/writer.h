#ifndef BDSP_STREAMS_STREAM_WRITER_H
#define BDSP_STREAMS_STREAM_WRITER_H

#include "BDSP/streams/abstract/mixins.h"
#include "BDSP/streams/types.h"

namespace BDSP::streams {
namespace core {
class StreamWriterCore: public IStreamWriter, public BDSP::streams::core::StreamReadyMixin {
private:
    stream_writer_t _write_handler = nullptr;
    void *_write_handler_context = nullptr;

protected:
    core::IStreamWritingStrategy *_strategy = nullptr;
    void _set_strategy(core::IStreamWritingStrategy &strategy) noexcept;

public:
    bool get_ready_status() final;
    write_status_t finish() final;
    void set_stream_writer(stream_writer_t writer, void *context_ptr) final;
    write_status_t write(uint8_t byte) final;
    write_status_t write(uint8_t *buffer_ptr, size_t size) final;
};
} // namespace core

template<typename TStrategy>
class StreamWriter: public core::StreamWriterCore {
public:
    StreamWriter() {
        _strategy = new TStrategy();
        _set_strategy(*_strategy);
    }

    ~StreamWriter() { delete _strategy; }

    TStrategy &get_strategy() { return *reinterpret_cast<TStrategy *>(_strategy); };
};

} // namespace BDSP::streams

#endif // BDSP_STREAMS_STREAM_WRITER_H
