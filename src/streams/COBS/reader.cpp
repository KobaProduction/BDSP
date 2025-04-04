#include "BDSP/streams/COBS/reader.h"
#include "BDSP/streams/COBS/checkers.h"

using namespace BDSP::streams;
using namespace BDSP::streams::COBS;
using namespace BDSP::streams::COBS::core;

uint8_t COBSReader::_get_converted_swap_byte_offset(uint8_t raw_offset) {
    return _cfg.delimiter not_eq 0x00 and raw_offset == 0x00 ? _cfg.delimiter : raw_offset;
}

read_status_t COBSReader::_process_byte(uint8_t byte) {
    read_status_t status = READ_OK;

    if (byte == _cfg.delimiter) {
        return _fsm_state not_eq REGULAR_BYTE ? READ_END : READ_ERROR;
    }

    if (_fsm_state not_eq REGULAR_BYTE) {
        status = _set_swap_byte_offset(byte);
        if (status == READ_ERROR) {
            return status;
        }
        if (_fsm_state == SWAP_BYTE) {
            _fsm_state = REGULAR_BYTE;
            byte = _cfg.delimiter;
        }
    }

    if (_fsm_state == REGULAR_BYTE) {
        _handler(byte, status);
    }

    _swap_byte_offset--;
    _service_byte_offset--;

    if (_service_byte_offset == 0) {
        _reset();
    } else if (_swap_byte_offset == 0) {
        _fsm_state = SWAP_BYTE;
    } else {
        _fsm_state = REGULAR_BYTE;
    }

    return status;
}

read_status_t COBSSRReader::_process_byte(uint8_t byte) {
    if (_fsm_state == REPLACEMENT_SEQUENCE and byte == _cfg.delimiter) {
        return READ_ERROR;
    }

    read_status_t status = COBSReader::_process_byte(byte);

    if (_next_swap_byte_is_place_of_the_replaced_sequence and status == READ_OK and _fsm_state == SWAP_BYTE) {
        _fsm_state = REPLACEMENT_SEQUENCE;
    }

    return status;
}

void COBSReader::_reset() {
    _fsm_state = SERVICE_BYTE;
    _service_byte_offset = _cfg.depth;
}

read_status_t COBSReader::_set_swap_byte_offset(uint8_t offset) {
    _swap_byte_offset = _get_converted_swap_byte_offset(offset);
    _service_byte_offset = _cfg.depth;
    return _swap_byte_offset > _cfg.depth ? READ_ERROR : READ_OK;
}

read_status_t COBSSRReader::_set_swap_byte_offset(uint8_t offset) {
    if (_fsm_state == REPLACEMENT_SEQUENCE) {
        for (int i = 0; i < _cfg.size_of_the_sequence_to_be_replaced; ++i) {
            _handler(_cfg.byte_of_the_sequence_to_be_replaced, READ_OK);
        }
    }

    _swap_byte_offset = _get_converted_swap_byte_offset(offset);
    _service_byte_offset = _cfg.depth;

    if (_swap_byte_offset > _sequence_replace_length_threshold) {
        _swap_byte_offset -= _sequence_replace_length_threshold;
        _next_swap_byte_is_place_of_the_replaced_sequence = true;
    } else {
        _next_swap_byte_is_place_of_the_replaced_sequence = false;
    }

    return _swap_byte_offset > _cfg.depth ? READ_ERROR : READ_OK;
}

COBS::cobs_config_t COBSReader::get_config() {
    return _cfg;
}

set_cobs_config_status COBSReader::set_config(cobs_config_t config) {
    _set_ready_state(false);

    set_cobs_config_status status = _config_checker(config);

    if (status not_eq SET_OK) {
        return status;
    }

    if (_fsm_state not_eq SERVICE_BYTE or _service_byte_offset not_eq _cfg.depth) {
        return ERROR_PROCESS_NOT_FINISHED;
    }

    if (config.depth < MIN_BDSP_COBS_DEPTH) {
        return ERROR_COBS_DEPTH;
    }

    _cfg = config;
    _reset();
    _set_ready_state(true);
    return status;
}
COBSReader::COBSReader() {
    _config_checker = cobs_default_config_checker;
    _cfg = {'\0', 255};
    _service_byte_offset = _cfg.depth;
    _reset();
}

COBSSRReader::COBSSRReader() {
    _config_checker = cobs_sr_config_checker;
    _sequence_replace_length_threshold = 127;
    _cfg = {'\0', 127, 2};
    _service_byte_offset = _cfg.depth;
}

COBSZPEReader::COBSZPEReader() {
    _config_checker = cobs_zpe_config_checker;
    _sequence_replace_length_threshold = 224;
    _cfg = {'\0', 224, 2};
    _service_byte_offset = _cfg.depth;
}
