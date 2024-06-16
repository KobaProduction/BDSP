#include "encoder.h"

COBSEncoder::COBSEncoder(cobs_config_t config, cobs_write_handler_t write_handler, void *write_handler_context_ptr) {
    _cfg = config;
    _write_handler = write_handler;
    _handler_context_ptr = write_handler_context_ptr;
    if (not _cfg.depth) _cfg.depth = DEFAULT_COBS_DEPTH;
    _buffer_ptr = reinterpret_cast<uint8_t*>(malloc(_cfg.depth));
    reset();
}

COBSEncoder::~COBSEncoder() {
    free(_buffer_ptr);
}

cobs_encoder_status_t COBSEncoder::get_status() {
    if (not _buffer_ptr) return COBS_BUFFER_MISSING;
    return COBS_OK;
}

cobs_encoder_status_t COBSEncoder::finish_encoding(bool is_send_with_delimiter) {
    uint8_t size = _current_buffer_ptr - _buffer_ptr;
    if (size < 2) return COBS_EMPTY_DATA;
    _write_handler(_buffer_ptr, size, _handler_context_ptr);
    if (is_send_with_delimiter) _write_handler(&_cfg.delimiter, 1, _handler_context_ptr);
    reset();
    return COBS_OK;
}

void COBSEncoder::send_segment(uint8_t *buffer_ptr, size_t size) {
    if (not _write_handler) return;
    uint8_t *current_byte_ptr = buffer_ptr;
    for (size_t i = 0; i < size; ++i) {
        uint8_t byte = *current_byte_ptr++;
        uint8_t bytes_in_buffer = _current_buffer_ptr - _buffer_ptr;
        if (byte != _cfg.delimiter and bytes_in_buffer < _cfg.depth) {
            *_current_buffer_ptr++ = byte;
            _service_byte_offset++;
            continue;
        } else if (bytes_in_buffer == _cfg.depth) {
            current_byte_ptr--; i--;
        }
        *_service_byte_ptr = _cfg.delimiter and _service_byte_offset == _cfg.delimiter ? 0 : _service_byte_offset;
        _write_handler(_buffer_ptr, bytes_in_buffer, _handler_context_ptr);
        reset();
    }
    *_service_byte_ptr = _cfg.delimiter and _service_byte_offset == _cfg.delimiter ? 0 : _current_buffer_ptr - _service_byte_ptr;
}

void COBSEncoder::send_byte(uint8_t &byte) {
    send_segment(&byte, 1);
}

void COBSEncoder::reset() {
    _service_byte_offset = 1;
    _current_buffer_ptr = _buffer_ptr;
    _service_byte_ptr = _current_buffer_ptr++;
}