#include "BDSP/streams/COBS/writer.h"
#include "BDSP/streams/COBS/checkers.h"

using namespace BDSP::core;
using namespace BDSP::streams;
using namespace BDSP::streams::COBS;
using namespace BDSP::streams::COBS::core;

void COBSWriterCore::_encode(uint8_t byte) {
    if (_buffer_position == _cfg.depth) {
        _write_buffer(_buffer_position);
    }

    if (byte not_eq _cfg.delimiter) {
        _buffer_ptr[_buffer_position++] = byte;
    } else {
        _write_buffer(_buffer_position);
    }
}

void COBSWriterCore::_finish() {
    if (_buffer_position == _cfg.depth) {
        _write_buffer(_buffer_position);
    }
    _write_buffer(_buffer_position);
    _write(_cfg.delimiter);
}

void COBSSRWriterCore::_finish() {
    if (_current_size_of_the_sequence_to_be_replaced) {
        _reset_elimination_sequence();
    }
    COBSWriterCore::_finish();
}

void COBSWriterCore::_process_byte(uint8_t byte) {
    _encode(byte);
}

void COBSSRWriterCore::_process_byte(uint8_t byte) {
    if (byte == _cfg.byte_of_the_sequence_to_be_replaced) {
        _current_size_of_the_sequence_to_be_replaced++;
        if (_current_size_of_the_sequence_to_be_replaced == _cfg.size_of_the_sequence_to_be_replaced) {
            _write_buffer(_buffer_position + 127);
            _current_size_of_the_sequence_to_be_replaced = 0;
        }
        return;
    } else if (_current_size_of_the_sequence_to_be_replaced) {
        _reset_elimination_sequence();
    }
    _encode(byte);
}

void COBSZPEWriterCore::_process_byte(uint8_t byte) {
    if (byte == _cfg.byte_of_the_sequence_to_be_replaced and _buffer_position < 31) {
        _current_size_of_the_sequence_to_be_replaced++;
        if (_current_size_of_the_sequence_to_be_replaced == _cfg.size_of_the_sequence_to_be_replaced) {
            _write_buffer(_buffer_position + 0xE0);
            _current_size_of_the_sequence_to_be_replaced = 0;
        }
        return;
    } else if (_current_size_of_the_sequence_to_be_replaced) {
        _reset_elimination_sequence();
    }
    _encode(byte);
}

void COBSSRWriterCore::_reset_elimination_sequence() {
    for (size_t i = 0; i < _current_size_of_the_sequence_to_be_replaced; ++i) {
        _encode(_cfg.byte_of_the_sequence_to_be_replaced);
    }
    _current_size_of_the_sequence_to_be_replaced = 0;
}

void COBSWriterCore::_write_buffer(uint8_t offset_value) {
    _buffer_ptr[0] = _cfg.delimiter not_eq 0x00 and offset_value == _cfg.delimiter ? 0 : offset_value;
    _write(_buffer_ptr, _buffer_position);
    _buffer_position = 1;
}

COBSWriterCore::~COBSWriterCore() {
    _free(_buffer_ptr);
}

COBS::cobs_config_t COBSWriterCore::get_config() {
    return _cfg;
}

set_config_status COBSWriterCore::_set_config(cobs_config_t config) {
    if (_buffer_position not_eq 1) {
        return ERROR_PROCESS_NOT_FINISHED;
    }

    set_config_status status = SET_OK;

    if (config.depth < MIN_BDSP_COBS_DEPTH) {
        return ERROR_COBS_DEPTH;
    }

    _set_ready_state(false);

    if (_buffer_ptr) {
        _free(_buffer_ptr);
        _buffer_ptr = nullptr;
    }

    _buffer_ptr = reinterpret_cast<uint8_t *>(_malloc(config.depth));

    if (not _buffer_ptr) {
        return ERROR_MEMORY_ALLOCATION;
    }

    _cfg = config;
    _set_ready_state(true);

    return status;
}

set_config_status COBSWriterCore::set_config(cobs_config_t config) {
    set_config_status status = cobs_default_config_checker(config);
    return status not_eq SET_OK ? status : _set_config(config);
}

set_config_status COBSSRWriterCore::set_config(COBS::cobs_config_t config) {
    set_config_status status = cobs_sr_config_checker(config);
    if (_current_size_of_the_sequence_to_be_replaced not_eq 0) {
        return ERROR_PROCESS_NOT_FINISHED;
    }
    return status not_eq SET_OK ? status : _set_config(config);
}

set_config_status COBSZPEWriterCore::set_config(cobs_config_t config) {
    set_config_status status = cobs_zpe_config_checker(config);
    if (_current_size_of_the_sequence_to_be_replaced not_eq 0) {
        return ERROR_PROCESS_NOT_FINISHED;
    }
    return status not_eq SET_OK ? status : _set_config(config);
}

COBSWriter::COBSWriter() noexcept {
    COBSWriterCore::set_config({'\0', 255});
}

COBSSRWriter::COBSSRWriter() noexcept {
    COBSSRWriterCore::set_config({'\0', 127, 2});
}

COBSZPEWriter::COBSZPEWriter() noexcept {
    COBSZPEWriterCore::set_config({'\0', 224, 2});
}