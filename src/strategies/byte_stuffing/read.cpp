#include "BDSP/streams/PPP/reader.h"

using namespace BDSP::strategies;
using namespace BDSP::strategies::byte_stuffing;
using namespace BDSP::strategies::byte_stuffing::core;

void ByteStuffingReadStrategyCore::_init() {
    _cfg = get_default_config();
}

strategy_read_status_t ByteStuffingReadStrategyCore::_send_status_and_reset(uint8_t byte,
                                                                            strategy_read_status_t status) {
    _data_callback(byte, status, _context);
    reset_read_state();
    return status;
}

byte_stuffing_config_t ByteStuffingReadStrategyCore::get_config() {
    return _cfg;
}

strategy_read_status_t ByteStuffingReadStrategyCore::read(uint8_t byte) {
    strategy_read_status_t status = STRATEGY_READ_OK;
    if (byte == _cfg.delimiter_byte) {
        if (_is_escape_state) {
            _data_callback(byte, STRATEGY_READ_ERROR, _context);
        }
        return _send_status_and_reset(byte, STRATEGY_READ_DELIMITER);
    }
    if (_is_escape_state) {
        _is_escape_state = false;
        byte ^= _cfg.escape_mask;
        if (byte not_eq _cfg.escape_byte and byte not_eq _cfg.delimiter_byte) {
            return _send_status_and_reset(byte, STRATEGY_READ_ERROR);
        }
    } else if (byte == _cfg.escape_byte) {
        _is_escape_state = true;
        return STRATEGY_READ_OK;
    }
    _data_callback(byte, status, _context);
    return status;
}

void ByteStuffingReadStrategyCore::reset_read_state() {
    _is_escape_state = false;
}

set_byte_stuffing_config_status ByteStuffingReadStrategyCore::set_config(byte_stuffing_config_t config) {
    set_byte_stuffing_config_status status = _check_config(config);
    if (_is_escape_state) {
        status = ERROR_PROCESS_NOT_FINISHED;
    }
    if (status == SET_OK) {
        _cfg = config;
    }
    _ready_state_callback(status == SET_OK, _context);
    return status;
}