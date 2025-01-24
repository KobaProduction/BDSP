#ifndef BDSP_STREAMS_ABSTRACT_READER_H
#define BDSP_STREAMS_ABSTRACT_READER_H

#include <stdint.h>

#include "BDSP/streams/abstract/stream.h"
#include "BDSP/streams/types.h"

namespace BDSP::streams::ABS {

class AbstractReader: public IReader, protected AbstractStreamReadyMixin {
    stream_data_handler_t _data_handler = nullptr;
    void *_data_handler_context = nullptr;
    bool _is_waiting_for_the_delimiter = false;

protected:
    void _handler(uint8_t byte, read_status_t state);
    virtual read_status_t _process_byte(uint8_t byte) = 0;
    virtual void _reset() = 0;

public:
    bool get_ready_status() override;
    read_status_t read(uint8_t byte) final;
    read_status_t read(uint8_t *buffer_ptr, size_t size) final;
    void reset_read_state(bool is_need_wait_delimiter) final;
    void set_stream_data_handler(stream_data_handler_t handler, void *context_ptr) final;
};
} // namespace BDSP::streams::ABS

#endif // BDSP_STREAMS_ABSTRACT_READER_H
