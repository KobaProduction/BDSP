#ifndef BDSP_STREAMS_ABSTRACT_WRITER_H
#define BDSP_STREAMS_ABSTRACT_WRITER_H

#include <stdint.h>

#include "BDSP/streams/abstract/stream.h"
#include "BDSP/streams/types.h"

namespace BDSP::streams::ABS {

class AbstractWriter: public IStreamWriter, protected AbstractStreamReadyMixin {
    stream_writer_t _writer = nullptr;
    void *_writer_context = nullptr;

protected:
    virtual void _finish() = 0;
    virtual void _process_byte(uint8_t byte) = 0;
    void _write(uint8_t byte);
    void _write(uint8_t *buffer_ptr, size_t size);

public:
    write_status_t finish() final;
    bool get_ready_status() override;
    void set_stream_writer(stream_writer_t writer, void *context_ptr) final;
    write_status_t write(uint8_t byte) final;
    write_status_t write(uint8_t *buffer_ptr, size_t size) final;
};
} // namespace BDSP::streams::ABS

#endif // BDSP_STREAMS_ABSTRACT_WRITER_H
