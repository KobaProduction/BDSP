#include "BDSP/strategies/cobs/read.h"

using namespace BDSP::strategies;
using namespace BDSP::strategies::cobs;
using namespace BDSP::strategies::cobs::core;

void COBSReadStrategyCore::_exec_delimiter(uint8_t byte) {
    if (_offset_to_service_byte not_eq 0) {
        _data_callback(byte, STRATEGY_READ_ERROR, _context);
    }
    _data_callback(byte, STRATEGY_READ_DELIMITER, _context);
    reset_read_state();
}

bool COBSReadStrategyCore::_exec_new_offset_and_get_success_status(uint8_t offset) {
    if (offset > _cfg.depth) {
        return false;
    }
    if (not _service_byte_type_is_offset) {
        _data_callback(_cfg.delimiter_byte, STRATEGY_READ_OK, _context);
    }
    _offset_to_service_byte = offset;
    _service_byte_type_is_offset = offset == _cfg.depth;
    return true;
}

inline uint8_t COBSReadStrategyCore::_get_converted_swap_byte_offset(uint8_t raw_offset) {
    return _cfg.delimiter_byte not_eq 0x00 and raw_offset == 0x00 ? _cfg.delimiter_byte : raw_offset;
}

bool COBSReadStrategyCore::_get_read_process_state() {
    if (not _service_byte_type_is_offset or _offset_to_service_byte not_eq 0) {
        return true;
    }
    return false;
}

void COBSReadStrategyCore::_init() {
    _cfg = get_default_config();
    reset_read_state();
}

cobs_config_t COBSReadStrategyCore::get_config() {
    return _cfg;
}

strategy_read_status_t COBSReadStrategyCore::read(uint8_t byte) {
    if (byte == _cfg.delimiter_byte) {
        _exec_delimiter(byte);
        return STRATEGY_READ_DELIMITER;
    }
    if (_offset_to_service_byte) {
        _data_callback(byte, STRATEGY_READ_OK, _context);
    } else {
        uint8_t offset = _get_converted_swap_byte_offset(byte);
        if (not _exec_new_offset_and_get_success_status(offset)) {
            _data_callback(byte, STRATEGY_READ_ERROR, _context);
            reset_read_state();
            return STRATEGY_READ_ERROR;
        }
    }
    _offset_to_service_byte--;
    return STRATEGY_READ_OK;
}

void COBSReadStrategyCore::reset_read_state() {
    _offset_to_service_byte = 0;
    _service_byte_type_is_offset = true;
}

set_cobs_config_status COBSReadStrategyCore::set_config(cobs_config_t config) {
    _ready_state_callback(false, _context);
    set_cobs_config_status status = _check_config(config);
    if (status not_eq SET_OK) {
        return status;
    }
    if (_get_read_process_state()) {
        return ERROR_PROCESS_NOT_FINISHED;
    }
    _cfg = config;
    reset_read_state();
    _ready_state_callback(true, _context);
    return status;
}

void COBSGSEReadStrategyCore::_exec_delimiter(uint8_t byte) {
    if (_offset_to_service_byte not_eq 0 or _is_sequence_replacement_state) {
        _data_callback(byte, STRATEGY_READ_ERROR, _context);
    }
    _data_callback(byte, STRATEGY_READ_DELIMITER, _context);
    reset_read_state();
}

bool COBSGSEReadStrategyCore::_exec_new_offset_and_get_success_status(uint8_t offset) {
    bool is_need_activate_sequence_replacement_state = offset > _cfg.depth;
    if (is_need_activate_sequence_replacement_state) {
        offset -= _cfg.depth;
    }
    if (offset > _cfg.depth) {
        return false;
    }
    if (_is_sequence_replacement_state) {
        for (int i = 0; i < _cfg.size_of_the_sequence_to_be_replaced; ++i) {
            _data_callback(_cfg.byte_of_the_sequence_to_be_replaced, STRATEGY_READ_OK, _context);
        }
    } else if (not _service_byte_type_is_offset) {
        _data_callback(_cfg.delimiter_byte, STRATEGY_READ_OK, _context);
    }
    _is_sequence_replacement_state = is_need_activate_sequence_replacement_state;
    _offset_to_service_byte = offset;
    _service_byte_type_is_offset = offset == _cfg.depth;
    return true;
}

void COBSGSEReadStrategyCore::reset_read_state() {
    _is_sequence_replacement_state = false;
    COBSReadStrategyCore::reset_read_state();
}

bool COBSGSEReadStrategyCore::_get_read_process_state() {
    if (_is_sequence_replacement_state) {
        return true;
    }
    return COBSReadStrategyCore::_get_read_process_state();
}