#include "BDSP/streams/abstract/writer.h"

using namespace BDSP::streams;
using namespace BDSP::streams::ABS;

void AbstractWriter::_write(uint8_t byte) {
    _writer(byte, _writer_context);
}

void AbstractWriter::_write(uint8_t *buffer_ptr, size_t size) {
    for (int i = 0; i < size; ++i) {
        _write(buffer_ptr[i]);
    }
}

write_status_t AbstractWriter::write(uint8_t byte) {
    if (_state not_eq READY) {
        return STREAM_WRITER_NOT_READY_ERROR;
    }
    _process_byte(byte);
    return STREAM_WRITE_OK;
}

write_status_t AbstractWriter::write(uint8_t *buffer_ptr, size_t size) {
    write_status_t status = STREAM_WRITE_OK;
    for (size_t i = 0; i < size; ++i) {
        status = write(buffer_ptr[i]);
        if (status not_eq STREAM_WRITE_OK) {
            break;
        }
    }
    return status;
}

write_status_t AbstractWriter::finish() {
    if (_state not_eq READY) {
        return STREAM_WRITER_NOT_READY_ERROR;
    }
    _finish();
    return STREAM_WRITE_END;
}

bool AbstractWriter::get_ready_status() {
    return _state == READY;
}

void AbstractWriter::set_stream_writer(stream_writer_t writer, void *context_ptr) {
    _writer = writer;
    _writer_context = context_ptr;
    _set_handler_state(_writer not_eq nullptr);
}
