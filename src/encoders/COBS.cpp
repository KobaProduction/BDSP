#include "stdlib.h"
#include "BDSP/encoders/COBS.h"

using namespace BDSP::encoders::COBS;

COBSEncoder::COBSEncoder(cobs_config_t config) {
    _cfg = config;
    if (_cfg.depth < MIN_BDSP_COBS_DEPTH) _cfg.depth = MIN_BDSP_COBS_DEPTH;
    _buffer_ptr = reinterpret_cast<uint8_t *>(malloc(_cfg.depth));
    if (not _buffer_ptr) _is_ready = false;
}

void COBSEncoder::_encode(uint8_t byte) {
    if (_buffer_position == _cfg.depth) _write_buffer();

    if (byte not_eq _cfg.delimiter) _buffer_ptr[_buffer_position++] = byte;
    else _write_buffer();
}

void COBSEncoder::_write_buffer() {
    _buffer_ptr[0] = _cfg.delimiter not_eq 0x00 and _buffer_position == _cfg.delimiter ? 0 : _buffer_position;
    _write(_buffer_ptr, _buffer_position);
    _buffer_position = 1;
}

COBSEncoder::~COBSEncoder() {
    free(_buffer_ptr);
}

encode_status_t COBSEncoder::finish_encode() {
    if (not _is_ready) return UNKNOWN_ENCODER_ERROR;
    if (_buffer_position == _cfg.depth) _write_buffer();
    _write_buffer();
    _write(_cfg.delimiter);
    return ENCODE_FINISH;
}