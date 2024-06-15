#include "decoder.h"

COBSDecoder::COBSDecoder(cobs_config_t config, cobs_decoder_data_callback_t data_callback, void *data_callback_context_ptr) {
    _cfg = config;
    _data_callback = data_callback;
    _callback_context_ptr = data_callback_context_ptr;
    reset();
}

void COBSDecoder::reset(bool is_wait_delimiter) {
    _fsm_state = is_wait_delimiter ? WAIT_DELIMITER : SERVICE_BYTE;
    _swap_byte_offset = 0;
    _service_byte_offset = _cfg.depth;
}

void COBSDecoder::parse(uint8_t byte) {
    switch (_fsm_state) {
        case SERVICE_BYTE:
            if (byte == _cfg.delimiter) return reset();
            _set_swap_byte_offset(byte);
            break;
        case REGULAR_BYTE:
            if (byte == _cfg.delimiter) {
                // Decoding error. The separator should not appear in regular data.
                return _set_error_state(byte);
            }
            _data_callback(byte, OK, _callback_context_ptr);
            break;
        case SWAP_BYTE:
            if (byte == _cfg.delimiter) {
                _data_callback(byte, END, _callback_context_ptr);
                return reset();
            }
            _set_swap_byte_offset(byte);
            _data_callback(_cfg.delimiter, OK, _callback_context_ptr);
            break;
        case WAIT_DELIMITER:
            if (byte == _cfg.delimiter) return reset();
            break;
    }
    if (_fsm_state == WAIT_DELIMITER) return;
    _service_byte_offset--;
    _swap_byte_offset--;
    if (_service_byte_offset == 0) {
        _fsm_state = SERVICE_BYTE;
        _service_byte_offset = _cfg.depth;
    } else if (_swap_byte_offset == 0) {
        _fsm_state = SWAP_BYTE;
    } else {
        _fsm_state = REGULAR_BYTE;
    }
}

void COBSDecoder::parse(uint8_t *buffer_ptr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        parse(buffer_ptr[i]);
    }
}

void COBSDecoder::_set_swap_byte_offset(uint8_t offset) {
    // Decoding error. The size of the new swap_byte_offset cannot exceed the swap_byte_offset from the configuration.
    if (offset > _cfg.depth) _set_error_state(offset);

    // Substitution of swap_byte_offset if the delimiter in the configuration is not equal to 0.
    _swap_byte_offset = not offset and _cfg.delimiter ? _cfg.delimiter : offset;
}

void COBSDecoder::_set_error_state(uint8_t byte) {
    _data_callback(byte, ERROR, _callback_context_ptr);
    _fsm_state = byte == _cfg.delimiter ? SERVICE_BYTE : WAIT_DELIMITER;
}