#ifndef BDSP_STREAMS_TYPES_H
#define BDSP_STREAMS_TYPES_H

#include <stdint.h>

namespace BDSP::streams {

enum stream_read_status_t {
    STREAM_READ_OK = 0,
    STREAM_READ_DELIMITER = 1,
    STREAM_READ_ERROR = 2,
    STREAM_READER_NOT_READY_ERROR = 3
};
enum stream_write_status_t { STREAM_WRITE_OK, STREAM_WRITE_DELIMITER, STREAM_WRITER_NOT_READY_ERROR };

typedef void (*stream_data_callback_t)(uint8_t byte, stream_read_status_t status, void *context_ptr);
typedef void (*stream_write_handler_t)(uint8_t byte, void *context_ptr);

class IStreamReader {
public:
    virtual stream_read_status_t read(uint8_t byte) = 0;
    virtual stream_read_status_t read(uint8_t *buffer_ptr, size_t size) = 0;
    virtual void reset_read_state(bool need_wait_delimiter) = 0;
    virtual void set_stream_data_handler(stream_data_callback_t handler, void *context_ptr) = 0;
    virtual bool get_ready_status() = 0;
};

class IStreamWriter {
public:
    virtual stream_write_status_t finish() = 0;
    virtual void set_stream_writer(stream_write_handler_t writer, void *context_ptr) = 0;
    virtual stream_write_status_t write(uint8_t byte) = 0;
    virtual stream_write_status_t write(uint8_t *buffer_ptr, size_t size) = 0;
    virtual bool get_ready_status() = 0;
};

} // namespace BDSP::streams

#endif // BDSP_STREAMS_TYPES_H
