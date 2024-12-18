#include "BDSP/decoders/abs_decoder.h"

using namespace BDSP::decoders::ABS;

void AbstractDecoder::set_data_handler(data_handler_t handler, void *context_ptr) {
    _data_handler = handler;
    _data_handler_context = context_ptr;
}

void AbstractDecoder::_handler(uint8_t byte, decode_status_t state) {
    if (_is_waiting_delemiter) return;
    _data_handler(byte, state, _data_handler_context);
}

decode_status_t AbstractDecoder::decode(uint8_t byte) {
    if (not _data_handler) return DATA_HANDLER_NOT_INSTALLED;
    decode_status_t state = _decode(byte);
    if (state == DECODE_ERROR) _is_waiting_delemiter = true;
    return state;
}

decode_status_t AbstractDecoder::decode(uint8_t *buffer_ptr, size_t size) {
    decode_status_t status = decode_status_t::DECODE_OK;
    for (size_t i = 0; i < size; ++i) {
        switch (decode(buffer_ptr[i])) {
            case DECODE_ERROR:
                status = DECODE_ERROR;
                break;
            case DATA_HANDLER_NOT_INSTALLED:
                return DATA_HANDLER_NOT_INSTALLED;
            default:
                break;
        }
    }
    return status;
}
