#ifndef BDSP_STREAMS_STREAM_READER_H
#define BDSP_STREAMS_STREAM_READER_H

#include "BDSP/streams/mixins.h"
#include "BDSP/streams/strategies/types.h"
#include "BDSP/streams/types.h"

namespace BDSP::streams {

namespace core {
class StreamReaderCore: public IStreamReader, public core::StreamReadyMixin {
    stream_data_callback_t _data_handler = nullptr;
    void *_data_handler_context = nullptr;
    bool _is_waiting_for_the_delimiter = false;

protected:
    strategies::IStreamReadStrategy *_strategy = nullptr;
    void _handler(uint8_t byte, stream_read_status_t state);

public:
    bool get_ready_status() override;
    stream_read_status_t read(uint8_t byte) final;
    stream_read_status_t read(uint8_t *buffer_ptr, size_t size) final;
    void reset_read_state(bool is_need_wait_delimiter) final;
    void set_strategy(strategies::IStreamReadStrategy &strategy) noexcept;
    void set_stream_data_handler(stream_data_callback_t handler, void *context_ptr) final;
};
} // namespace core

template<typename TStrategy>
class StreamReader: public core::StreamReaderCore {
protected:
    using core::StreamReaderCore::set_strategy;

public:
    StreamReader() {
        _strategy = new TStrategy();
        set_strategy(*_strategy);
    }

    ~StreamReader() { delete _strategy; }

    TStrategy &get_strategy() { return *reinterpret_cast<TStrategy *>(_strategy); };
};

} // namespace BDSP::streams

#endif // BDSP_STREAMS_STREAM_READER_H
