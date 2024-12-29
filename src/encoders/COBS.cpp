#include "stdlib.h"
#include "BDSP/encoders/COBS.h"

using namespace BDSP::encoders::COBS;


void COBSEncoder::_encode_default(uint8_t byte) {
    if (_buffer_position == _cfg.depth) _write_buffer();

    if (byte not_eq _cfg.delimiter) _buffer_ptr[_buffer_position++] = byte;
    else _write_buffer();
}

void COBSEncoder::_encode(uint8_t byte) {
    if (_cfg.size_of_the_sequence_to_be_replaced and byte == _cfg.byte_of_the_sequence_to_be_replaced) {
        _current_size_of_the_sequence_to_be_replaced++;
        if (_current_size_of_the_sequence_to_be_replaced == _cfg.size_of_the_sequence_to_be_replaced) {
            _write_buffer(true);
            _current_size_of_the_sequence_to_be_replaced = 0;
        }
        return;
    }

    if (_current_size_of_the_sequence_to_be_replaced and byte not_eq _cfg.byte_of_the_sequence_to_be_replaced) {
        _reset_elimination_sequence();
    }
    _encode_default(byte);
}

void COBSEncoder::_finish_encode() {
    if (_current_size_of_the_sequence_to_be_replaced) _reset_elimination_sequence();
    if (_buffer_position == _cfg.depth) _write_buffer();
    _write_buffer();
    _write(_cfg.delimiter);
}

void COBSEncoder::_reset_elimination_sequence() {
    for (size_t i = 0; i < _current_size_of_the_sequence_to_be_replaced; ++i) {
        _encode_default(_cfg.byte_of_the_sequence_to_be_replaced);
    }
    _current_size_of_the_sequence_to_be_replaced = 0;
}

void COBSEncoder::_write_buffer(bool is_elimination_sequence) {
    _buffer_ptr[0] = _cfg.delimiter not_eq 0x00 and _buffer_position == _cfg.delimiter ? 0 : _buffer_position;
    if (is_elimination_sequence) {
        _buffer_ptr[0] += 127;
    }
    _write(_buffer_ptr, _buffer_position);
    _buffer_position = 1;
}

COBSEncoder::COBSEncoder(cobs_config_t config) {
    _cfg = config;

    if (_cfg.size_of_the_sequence_to_be_replaced < 2) {
        // todo return error status
        _cfg.size_of_the_sequence_to_be_replaced = 0;
    }

    if (_cfg.size_of_the_sequence_to_be_replaced and _cfg.depth > 127) {
        // todo return error status
        _cfg.depth = 127;
    }

    if (_cfg.depth < MIN_BDSP_COBS_DEPTH) _cfg.depth = MIN_BDSP_COBS_DEPTH;
    _buffer_ptr = reinterpret_cast<uint8_t *>(malloc(_cfg.depth));
    if (not _buffer_ptr) _is_ready = false;
}

COBSEncoder::~COBSEncoder() {
    free(_buffer_ptr);
}
