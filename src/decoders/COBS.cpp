#include "BDSP/decoders/COBS.h"

using namespace BDSP::decoders::COBS;

decode_status_t COBSDecoder::_decode(uint8_t byte) {
    decode_status_t status = DECODE_OK;

    if (byte == _cfg.delimiter) {
        return _fsm_state not_eq REGULAR_BYTE ? DECODE_END : DECODE_ERROR;
    }

    if (_fsm_state not_eq REGULAR_BYTE) {
        status = _set_swap_byte_offset(byte);
        if (status == DECODE_ERROR) return status;
        if (_fsm_state == SWAP_BYTE) byte = _cfg.delimiter;
    }

    if (_fsm_state not_eq SERVICE_BYTE) {
        _handler(byte, status);
    }

    _swap_byte_offset--;
    _service_byte_offset--;

    if (_service_byte_offset == 0) _reset();
    else if (_swap_byte_offset == 0) _fsm_state = SWAP_BYTE;
    else _fsm_state = REGULAR_BYTE;

    return status;
}

decode_status_t COBSDecoder::_set_swap_byte_offset(uint8_t offset) {
    // Substitution of swap_byte_offset if the delimiter in the configuration is not equal to 0.
    _swap_byte_offset = _cfg.delimiter not_eq 0x00 and offset == 0x00 ? _cfg.delimiter : offset;
    _service_byte_offset = _cfg.depth;

    // Decoding error. The size of the new swap_byte_offset cannot exceed the swap_byte_offset from the configuration.
    return _swap_byte_offset > _cfg.depth ? DECODE_ERROR : DECODE_OK;
}

void COBSDecoder::_reset() {
    _fsm_state = SERVICE_BYTE;
    _swap_byte_offset = 0xFF;
    _service_byte_offset = _cfg.depth;
}

COBSDecoder::COBSDecoder(cobs_config_t config) {
    _cfg = config;
    if (_cfg.depth < MIN_BDSP_COBS_DEPTH) _cfg.depth = MIN_BDSP_COBS_DEPTH;
    _reset();
}

