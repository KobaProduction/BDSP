#include "abs_decoder.h"

using namespace BDSP::decoders;

void ABS::AbstractDecoder::set_data_handler(data_handler_t handler, void *context_ptr) {
    _data_handler = handler;
    _data_handler_context = context_ptr;
}

void ABS::AbstractDecoder::_handler(uint8_t byte, decode_state_t state) {
    if (is_waiting_delemiter) return;
    _data_handler(byte, state, _data_handler_context);
}

decode_state_t ABS::AbstractDecoder::decode(uint8_t byte) {
    if (not _data_handler) return DATA_HANDLER_NOT_INSTALLED;

    decode_state_t state = _decode(byte);
    switch (state) {
        case DECODE_ERROR:
            is_waiting_delemiter = true;
            state = WAITING_FOR_DELIMITER;
            break;
        case DECODE_END:
            is_waiting_delemiter = false;
            break;
        default:
            break;
    }
    if (is_waiting_delemiter) return WAITING_FOR_DELIMITER;
    return state;
}

[[maybe_unused]] decode_state_t ABS::AbstractDecoder::decode(uint8_t *buffer_ptr, size_t size) {
    decode_state_t status = decode_state_t::DECODE_OK;
    for (size_t i = 0; i < size; ++i) {
        status = decode(buffer_ptr[i]);
        switch (status) {
            case DECODE_OK:
            case DECODE_END:
                break;
            case DECODE_ERROR:
            case WAITING_FOR_DELIMITER:
                status = DECODE_ERROR;
                break;
            case DATA_HANDLER_NOT_INSTALLED:
                return DATA_HANDLER_NOT_INSTALLED;
        }
    }
    return status;
}
