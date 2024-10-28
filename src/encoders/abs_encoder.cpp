#include "BDSP/encoders/abs_encoder.h"

using namespace BDSP::encoders::ABS;

void AbstractEncoder::set_writer(write_handler_t writer, void *context_ptr) {
    _writer = writer;
    _writer_context = context_ptr;
}

encode_status_t AbstractEncoder::encode(uint8_t byte) {
    if (not _writer) return encode_status_t::WRITER_NOT_INSTALLED_ERROR;
    _encode(byte);
    return encode_status_t::ENCODE_OK;
}

[[maybe_unused]] encode_status_t AbstractEncoder::encode(uint8_t *buffer_ptr, size_t size) {
    encode_status_t status = encode_status_t::ENCODE_OK;
    for (size_t i = 0; i < size; ++i) {
        status = encode(buffer_ptr[i]);
        if (status not_eq encode_status_t::ENCODE_OK) break;
    }
    return status;
}
