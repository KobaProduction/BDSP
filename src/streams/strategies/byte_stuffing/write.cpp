#include "BDSP/streams/strategies/byte_stuffing/write.h"

using namespace BDSP::streams::strategies::byte_stuffing;
using namespace BDSP::streams::strategies::byte_stuffing::core;

void ByteStuffingWriteStrategyCore::_init() {
    _cfg = get_default_config();
}

byte_stuffing_config_t ByteStuffingWriteStrategyCore::get_config() {
    return _cfg;
}

void ByteStuffingWriteStrategyCore::send_delimiter() {
    _write_handler(_cfg.delimiter_byte, _context);
}

set_byte_stuffing_config_status ByteStuffingWriteStrategyCore::set_config(byte_stuffing_config_t config) {
    set_byte_stuffing_config_status status = _check_config(config);
    if (status == SET_OK) {
        _cfg = config;
    }
    _ready_state_callback(status == SET_OK, _context);
    return status;
}

void ByteStuffingWriteStrategyCore::write(uint8_t byte) {
    if (byte not_eq _cfg.delimiter_byte and byte not_eq _cfg.escape_byte) {
        return _write_handler(byte, _context);
    }
    _write_handler(_cfg.escape_byte, _context);
    _write_handler(byte ^ _cfg.escape_mask, _context);
}