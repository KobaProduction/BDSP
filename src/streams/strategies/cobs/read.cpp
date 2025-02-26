#include "BDSP/streams/strategies/COBS/read.h"

using namespace BDSP::streams::strategies;
using namespace BDSP::streams::strategies::cobs;
using namespace BDSP::streams::strategies::cobs::core;

void COBSReadStrategyCore::_exec_delimiter(uint8_t byte) {
    if (_fsm_state == REGULAR_BYTE) {
        _data_callback(byte, STRATEGY_READ_ERROR, _context);
    }
    _data_callback(byte, STRATEGY_READ_DELIMITER, _context);
    reset_read_state();
}

uint8_t COBSReadStrategyCore::_get_converted_swap_byte_offset(uint8_t raw_offset) {
    return _cfg.delimiter_byte not_eq 0x00 and raw_offset == 0x00 ? _cfg.delimiter_byte : raw_offset;
}

bool COBSReadStrategyCore::_get_read_process_state() {
    if (_fsm_state not_eq SERVICE_BYTE or _service_byte_offset not_eq _cfg.depth) {
        return true;
    }
    return false;
}

void COBSReadStrategyCore::_init() {
    _cfg = get_default_config();
    reset_read_state();
}

strategy_read_status_t COBSReadStrategyCore::_set_swap_byte_offset(uint8_t offset) {
    offset = _get_converted_swap_byte_offset(offset);
    if (offset > _cfg.depth) {
        return STRATEGY_READ_ERROR;
    }
    _swap_byte_offset = offset;
    _service_byte_offset = _cfg.depth;
    return STRATEGY_READ_OK;
}

cobs_config_t COBSReadStrategyCore::get_config() {
    return _cfg;
}

strategy_read_status_t COBSReadStrategyCore::read(uint8_t byte) {
    strategy_read_status_t status = STRATEGY_READ_OK;
    if (byte == _cfg.delimiter_byte) {
        _exec_delimiter(byte);
        return STRATEGY_READ_DELIMITER;
    }
    if (_fsm_state not_eq REGULAR_BYTE) {
        status = _set_swap_byte_offset(byte);
        if (status == STRATEGY_READ_ERROR) {
            _data_callback(byte, status, _context);
            reset_read_state();
            return status;
        }
        if (_fsm_state == SWAP_BYTE) {
            _fsm_state = REGULAR_BYTE;
            byte = _cfg.delimiter_byte;
        }
    }
    if (_fsm_state == REGULAR_BYTE) {
        _data_callback(byte, status, _context);
    }
    _swap_byte_offset--;
    _service_byte_offset--;
    if (_service_byte_offset == 0) {
        reset_read_state();
    } else if (_swap_byte_offset == 0) {
        _fsm_state = SWAP_BYTE;
    } else {
        _fsm_state = REGULAR_BYTE;
    }
    return status;
}

void COBSReadStrategyCore::reset_read_state() {
    _fsm_state = SERVICE_BYTE;
    _service_byte_offset = _cfg.depth;
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

void COBSSRReadStrategyCore::_exec_delimiter(uint8_t byte) {
    if (_is_sequence_replacement_state) {
        _data_callback(byte, STRATEGY_READ_ERROR, _context);
    }
    COBSReadStrategyCore::_exec_delimiter(byte);
}

strategy_read_status_t COBSSRReadStrategyCore::_set_swap_byte_offset(uint8_t offset) {
    offset = _get_converted_swap_byte_offset(offset);
    bool is_need_activate_sequence_replacement_state = offset > _cfg.depth;
    if (is_need_activate_sequence_replacement_state) {
        offset -= _cfg.depth;
    }
    if (offset > _cfg.depth) {
        return STRATEGY_READ_ERROR;
    }
    _swap_byte_offset = offset;
    if (_is_sequence_replacement_state) {
        for (int i = 0; i < _cfg.size_of_the_sequence_to_be_replaced; ++i) {
            _data_callback(_cfg.byte_of_the_sequence_to_be_replaced, STRATEGY_READ_OK, _context);
        }
    }
    _is_sequence_replacement_state = is_need_activate_sequence_replacement_state;
    _service_byte_offset = _cfg.depth;
    return STRATEGY_READ_OK;
}

strategy_read_status_t COBSSRReadStrategyCore::read(uint8_t byte) {
    strategy_read_status_t status = COBSReadStrategyCore::read(byte);
    if (_is_sequence_replacement_state and _fsm_state == SWAP_BYTE) {
        _fsm_state = SERVICE_BYTE;
    }
    return status;
}

void COBSSRReadStrategyCore::reset_read_state() {
    COBSReadStrategyCore::reset_read_state();
    _is_sequence_replacement_state = false;
}

bool COBSSRReadStrategyCore::_get_read_process_state() {
    if (_is_sequence_replacement_state) {
        return true;
    }
    return COBSReadStrategyCore::_get_read_process_state();
}
