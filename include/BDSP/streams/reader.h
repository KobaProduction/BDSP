#ifndef BDSP_STREAMS_STREAM_READER_H
#define BDSP_STREAMS_STREAM_READER_H

#include "BDSP/streams/abstract/mixins.h"
#include "BDSP/streams/types.h"

namespace BDSP::streams {

namespace core {
class StreamReaderCore: public IStreamReader, public BDSP::streams::core::StreamReadyMixin {
    stream_data_handler_t _data_handler = nullptr;
    void *_data_handler_context = nullptr;
    bool _is_waiting_for_the_delimiter = false;

protected:
    core::IStreamReadingStrategy *_strategy = nullptr;
    void _handler(uint8_t byte, read_status_t state);
    void _set_strategy(core::IStreamReadingStrategy &strategy) noexcept;

public:
    bool get_ready_status() override;
    read_status_t read(uint8_t byte) final;
    read_status_t read(uint8_t *buffer_ptr, size_t size) final;
    void reset_read_state(bool is_need_wait_delimiter) final;
    void set_stream_data_handler(stream_data_handler_t handler, void *context_ptr) final;
};
} // namespace core

template<typename TStrategy>
class StreamReader: public core::StreamReaderCore {
public:
    StreamReader() {
        _strategy = new TStrategy();
        _set_strategy(*_strategy);
    }

    ~StreamReader() { delete _strategy; }

    TStrategy &get_strategy() { return *reinterpret_cast<TStrategy *>(_strategy); };
};

} // namespace BDSP::streams

#endif // BDSP_STREAMS_STREAM_READER_H
