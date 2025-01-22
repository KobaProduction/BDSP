#include "BDSP/streams/COBS/writer.h"
#include "BDSP/streams/COBS/checkers.h"

using namespace BDSP::core;
using namespace BDSP::streams;
using namespace BDSP::streams::COBS;

void COBSWriter::_encode(uint8_t byte) {
    if (_buffer_position == _cfg.depth) {
        _write_buffer();
    }

    if (byte not_eq _cfg.delimiter) {
        _buffer_ptr[_buffer_position++] = byte;
    } else {
        _write_buffer();
    }
}

void COBSWriter::_finish() {
    if (_buffer_position == _cfg.depth) {
        _write_buffer();
    }
    _write_buffer();
    _write(_cfg.delimiter);
}

void COBSSRWriter::_finish() {
    if (_current_size_of_the_sequence_to_be_replaced) {
        _reset_elimination_sequence();
    }
    COBSWriter::_finish();
}

void COBSWriter::_process_byte(uint8_t byte) {
    _encode(byte);
}

void COBSSRWriter::_process_byte(uint8_t byte) {
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

void COBSZPEWriter::_process_byte(uint8_t byte) {
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

void COBSSRWriter::_reset_elimination_sequence() {
    for (size_t i = 0; i < _current_size_of_the_sequence_to_be_replaced; ++i) {
        _encode(_cfg.byte_of_the_sequence_to_be_replaced);
    }
    _current_size_of_the_sequence_to_be_replaced = 0;
}

void COBSWriter::_write_buffer(uint8_t cobs_offset_value) {
    if (not cobs_offset_value) {
        cobs_offset_value = _buffer_position;
    }
    _buffer_ptr[0] = _cfg.delimiter not_eq 0x00 and cobs_offset_value == _cfg.delimiter ? 0 : cobs_offset_value;
    _write(_buffer_ptr, _buffer_position);
    _buffer_position = 1;
}

COBSWriter::~COBSWriter() {
    _free(_buffer_ptr);
}

COBSWriter::COBSWriter() {
    _set_config = core::cobs_default_config_checker;
    COBSWriter::set_config({'\0', 255});
}

COBSSRWriter::COBSSRWriter() {
    _set_config = core::cobs_sr_config_checker;
    COBSWriter::set_config({'\0', 127, 2});
}

COBSZPEWriter::COBSZPEWriter() {
    _set_config = core::cobs_zpe_config_checker;
    COBSZPEWriter::set_config({'\0', 224, 2});
}

COBS::cobs_config_t COBSWriter::get_config() {
    return _cfg;
}

set_config_status COBSWriter::set_config(COBS::cobs_config_t config) {
    _is_ready = false;

    if (_buffer_position not_eq 1) {
        return ERROR_PROCESS_NOT_FINISHED;
    }

    set_config_status status = SET_OK;

    if (config.depth < MIN_BDSP_COBS_DEPTH) {
        status = WARNING_COBS_DEPTH;
        config.depth = MIN_BDSP_COBS_DEPTH;
    }

    if (not _set_config(config, status)) {
        return status;
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
    _is_ready = true;

    return status;
}
