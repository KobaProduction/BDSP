#include "BDSP/streams/strategies/cobs/write.h"

using namespace BDSP::streams::strategies::cobs;
using namespace BDSP::streams::strategies::cobs::core;

inline void COBSWriteStrategyCore::_check_buffer_fullness() {
    if (_buffer_position == _cfg.depth) {
        _write_buffer_to_stream(_buffer_position);
    }
}

inline void COBSWriteStrategyCore::_encode(uint8_t byte) {
    _check_buffer_fullness();
    if (byte not_eq _cfg.delimiter_byte) {
        _buffer_ptr[_buffer_position++] = byte;
    } else {
        _write_buffer_to_stream(_buffer_position);
    }
}

bool COBSWriteStrategyCore::_get_read_process_state() {
    if (_buffer_position not_eq 1) {
        return true;
    }
    return false;
}

void COBSWriteStrategyCore::_init() {
    set_config(get_default_config());
}

void COBSWriteStrategyCore::_write_buffer_to_stream(uint8_t offset_value) {
    _buffer_ptr[0] = _cfg.delimiter_byte not_eq 0x00 and offset_value == _cfg.delimiter_byte ? 0 : offset_value;
    for (int i = 0; i < _buffer_position; ++i) {
        _write_handler(_buffer_ptr[i], _context);
    }
    _buffer_position = 1;
}

COBSWriteStrategyCore::~COBSWriteStrategyCore() {
    _free(_buffer_ptr);
}

cobs_config_t COBSWriteStrategyCore::get_config() {
    return _cfg;
}

void COBSWriteStrategyCore::finish() {
    _check_buffer_fullness();
    _write_buffer_to_stream(_buffer_position);
    send_delimiter();
}

void COBSWriteStrategyCore::send_delimiter() {
    _write_handler(_cfg.delimiter_byte, _context);
}

set_cobs_config_status COBSWriteStrategyCore::set_config(cobs_config_t config) {
    _ready_state_callback(false, _context);
    set_cobs_config_status status = _check_config(config);
    if (status not_eq SET_OK) {
        return status;
    }
    if (_get_read_process_state()) {
        return ERROR_PROCESS_NOT_FINISHED;
    }
    if (_buffer_ptr and _cfg.depth not_eq config.depth) {
        _free(_buffer_ptr);
        _buffer_ptr = nullptr;
    }
    if (not _buffer_ptr) {
        _buffer_ptr = reinterpret_cast<uint8_t *>(_malloc(config.depth));
    }
    if (not _buffer_ptr) {
        return ERROR_MEMORY_ALLOCATION;
    }
    _cfg = config;
    _ready_state_callback(true, _context);
    return status;
}

void COBSWriteStrategyCore::write(uint8_t byte) {
    _encode(byte);
}

bool COBSSRWriteStrategyCore::_get_read_process_state() {
    if (_current_size_of_the_sequence_to_be_replaced not_eq 0) {
        return ERROR_PROCESS_NOT_FINISHED;
    }
    return COBSWriteStrategyCore::_get_read_process_state();
}

inline void COBSSRWriteStrategyCore::_reset_elimination_sequence() {
    while (_current_size_of_the_sequence_to_be_replaced) {
        _encode(_cfg.byte_of_the_sequence_to_be_replaced);
        _current_size_of_the_sequence_to_be_replaced--;
    }
}

void COBSSRWriteStrategyCore::finish() {
    if (_current_size_of_the_sequence_to_be_replaced) {
        _reset_elimination_sequence();
    }
    COBSWriteStrategyCore::finish();
}

set_cobs_config_status COBSSRWriteStrategyCore::set_config(cobs_config_t config) {
    set_cobs_config_status status = COBSWriteStrategyCore::set_config(config);
    if (status == SET_OK) {
        _position_threshold_of_the_sequence_to_be_replaced = 0xFF - _cfg.depth;
    }
    return status;
}

void COBSSRWriteStrategyCore::write(uint8_t byte) {
    if (byte == _cfg.byte_of_the_sequence_to_be_replaced and
        _buffer_position <= _position_threshold_of_the_sequence_to_be_replaced) {
        _current_size_of_the_sequence_to_be_replaced++;
        if (_current_size_of_the_sequence_to_be_replaced == _cfg.size_of_the_sequence_to_be_replaced) {
            _write_buffer_to_stream(_buffer_position + _cfg.depth);
            _current_size_of_the_sequence_to_be_replaced = 0;
        }
        return;
    } else if (_current_size_of_the_sequence_to_be_replaced) {
        _reset_elimination_sequence();
    }
    _encode(byte);
}
