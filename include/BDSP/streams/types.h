#ifndef BDSP_STREAMS_TYPES_H
#define BDSP_STREAMS_TYPES_H

#include <stdint.h>

namespace BDSP::streams {

enum read_status_t { STREAM_READ_OK, STREAM_READ_END, STREAM_READ_ERROR, ERROR_READ_STREAM_NOT_READY };

typedef void (*stream_data_handler_t)(uint8_t byte, read_status_t status, void *context_ptr);

class IReader {
public:
    virtual read_status_t read(uint8_t byte) = 0;
    virtual read_status_t read(uint8_t *buffer_ptr, size_t size) = 0;
    virtual void reset_read_state(bool need_wait_delimiter) = 0;
    virtual void set_stream_data_handler(stream_data_handler_t handler, void *context_ptr) = 0;
    virtual bool get_ready_status() = 0;
};

enum write_status_t { STREAM_WRITE_OK, STREAM_WRITE_END, ERROR_WRITE_STREAM_NOT_READY };

typedef void (*stream_writer_t)(uint8_t byte, void *context_ptr);

class IWriter {
public:
    virtual write_status_t finish() = 0;
    virtual void set_stream_writer(stream_writer_t writer, void *context_ptr) = 0;
    virtual write_status_t write(uint8_t byte) = 0;
    virtual write_status_t write(uint8_t *buffer_ptr, size_t size) = 0;
    virtual bool get_ready_status() = 0;
};

} // namespace BDSP::streams

#endif // BDSP_STREAMS_TYPES_H
