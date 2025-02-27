#include "BDSP/strategies/cobs/write.h"

using namespace BDSP::strategies::cobs;
using namespace BDSP::strategies::cobs::core;

inline void COBSWriteStrategyCore::_encode(uint8_t byte) {
    if (byte not_eq _cfg.delimiter_byte) {
        _buffer_ptr[_buffer_position++] = byte;
        if (_buffer_position < _cfg.depth) {
            return;
        }
    }
    _write_buffer_with_offset_to_handler(_buffer_position);
}

bool COBSWriteStrategyCore::_get_active_write_state_status() {
    if (_buffer_position not_eq 1) {
        return true;
    }
    return false;
}

void COBSWriteStrategyCore::_init() {
    set_config(get_default_config());
}

inline void COBSWriteStrategyCore::_write_buffer_with_offset_to_handler(uint8_t offset) {
    _buffer_ptr[0] = _cfg.delimiter_byte not_eq 0x00 and offset == _cfg.delimiter_byte ? 0 : offset;
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
    _write_buffer_with_offset_to_handler(_buffer_position);
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
    if (_get_active_write_state_status()) {
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

bool COBSSRWriteStrategyCore::_get_active_write_state_status() {
    if (_counter_of_the_sequence_to_be_replaced not_eq 0) {
        return ERROR_PROCESS_NOT_FINISHED;
    }
    return COBSWriteStrategyCore::_get_active_write_state_status();
}

inline void COBSSRWriteStrategyCore::_reset_counter_of_the_sequence_to_be_replaced() {
    while (_counter_of_the_sequence_to_be_replaced) {
        _encode(_cfg.byte_of_the_sequence_to_be_replaced);
        _counter_of_the_sequence_to_be_replaced--;
    }
}

void COBSSRWriteStrategyCore::finish() {
    if (_counter_of_the_sequence_to_be_replaced) {
        _reset_counter_of_the_sequence_to_be_replaced();
    }
    COBSWriteStrategyCore::finish();
}

set_cobs_config_status COBSSRWriteStrategyCore::set_config(cobs_config_t config) {
    set_cobs_config_status status = COBSWriteStrategyCore::set_config(config);
    if (status == SET_OK) {
        _limit_position_of_the_sequence_to_be_replaced = 0xFF - _cfg.depth;
    }
    return status;
}

void COBSSRWriteStrategyCore::write(uint8_t byte) {
    if (byte == _cfg.byte_of_the_sequence_to_be_replaced and
        _buffer_position <= _limit_position_of_the_sequence_to_be_replaced) {
        _counter_of_the_sequence_to_be_replaced++;
        if (_counter_of_the_sequence_to_be_replaced == _cfg.size_of_the_sequence_to_be_replaced) {
            _write_buffer_with_offset_to_handler(_buffer_position + _cfg.depth);
            _counter_of_the_sequence_to_be_replaced = 0;
        }
        return;
    } else if (_counter_of_the_sequence_to_be_replaced) {
        _reset_counter_of_the_sequence_to_be_replaced();
    }
    _encode(byte);
}
