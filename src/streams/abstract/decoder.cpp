#include "BDSP/streams/abstract/decoder.h"

using namespace BDSP::streams;
using namespace BDSP::streams::ABS;

void AbstractDecoder::set_data_handler(data_handler_t handler, void *context_ptr) {
    _data_handler = handler;
    _data_handler_context = context_ptr;
}

void AbstractDecoder::_handler(uint8_t byte, decode_status_t status) {
    if (_is_waiting_for_the_delemiter) return;
    _data_handler(byte, status, _data_handler_context);
}

decode_status_t AbstractDecoder::decode(uint8_t byte) {
    if (not _data_handler) return UNKNOWN_DECODER_ERROR;
    decode_status_t status = _decode(byte);
    if (status not_eq DECODE_OK) {
        _data_handler(byte, status, _data_handler_context);
        reset_decode_state(status == DECODE_ERROR);
    }
    return status;
}

decode_status_t AbstractDecoder::decode(uint8_t *buffer_ptr, size_t size) {
    decode_status_t status = decode_status_t::DECODE_OK;
    for (size_t i = 0; i < size; ++i) {
        switch (decode(buffer_ptr[i])) {
            case UNKNOWN_DECODER_ERROR:
                return UNKNOWN_DECODER_ERROR;
            case DECODE_ERROR:
                status = DECODE_ERROR;
            default:
                break;
        }
    }
    return status;
}

void AbstractDecoder::reset_decode_state(bool is_need_wait_delemiter) {
    _is_waiting_for_the_delemiter = is_need_wait_delemiter;
    _reset();
}
