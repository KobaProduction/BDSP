#include "BDSP/encoders/abs_encoder.h"

using namespace BDSP::encoders::ABS;

void AbstractEncoder::_write(uint8_t byte) {
    _writer(byte, _writer_context);
}

void AbstractEncoder::_write(uint8_t *buffer_ptr, size_t size) {
    for (int i = 0; i < size; ++i) _write(buffer_ptr[i]);
}

void AbstractEncoder::set_writer(write_handler_t writer, void *context_ptr) {
    _writer = writer;
    _writer_context = context_ptr;
}

encode_status_t AbstractEncoder::encode(uint8_t byte) {
    if (not _writer or not _is_ready) return UNKNOWN_ENCODER_ERROR;
    _encode(byte);
    return ENCODE_OK;
}

encode_status_t AbstractEncoder::encode(uint8_t *buffer_ptr, size_t size) {
    encode_status_t status = ENCODE_OK;
    for (size_t i = 0; i < size; ++i) {
        status = encode(buffer_ptr[i]);
        if (status not_eq ENCODE_OK) break;
    }
    return status;
}
