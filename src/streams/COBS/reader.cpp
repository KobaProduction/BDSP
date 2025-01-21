#include "BDSP/streams/COBS/reader.h"

using namespace BDSP::streams;
using namespace BDSP::streams::COBS;
using namespace BDSP::streams::COBS::core;

read_status_t COBSReader::_process_byte(uint8_t byte) {
    read_status_t status = READ_OK;

    if (byte == _cfg.delimiter) {
        return _fsm_state not_eq REGULAR_BYTE ? READ_END : READ_ERROR;
    }

    if (_fsm_state not_eq REGULAR_BYTE) {
        if (_fsm_state == REPLACEMENT_SEQUENCE) {
            for (int i = 0; i < _cfg.size_of_the_sequence_to_be_replaced; ++i) {
                _handler(_cfg.byte_of_the_sequence_to_be_replaced, READ_OK);
            }
        }
        status = _set_swap_byte_offset(byte);
        if (status == READ_ERROR) {
            return status;
        }
        if (_fsm_state == SWAP_BYTE) {
            byte = _cfg.delimiter;
        }
    }

    if (_fsm_state not_eq SERVICE_BYTE and _fsm_state not_eq REPLACEMENT_SEQUENCE) {
        _handler(byte, status);
    }

    _swap_byte_offset--;
    _service_byte_offset--;

    if (_service_byte_offset == 0) {
        _reset();
    } else if (_swap_byte_offset == 0) {
        _fsm_state = next_swap_byte_is_place_of_the_replaced_sequence ? REPLACEMENT_SEQUENCE : SWAP_BYTE;
    } else {
        _fsm_state = REGULAR_BYTE;
    }

    return status;
}

read_status_t COBSReader::_set_swap_byte_offset(uint8_t offset) {
    // Substitution of swap_byte_offset if the delimiter in the configuration is not equal to 0.
    _swap_byte_offset = _cfg.delimiter not_eq 0x00 and offset == 0x00 ? _cfg.delimiter : offset;
    if (_cfg.size_of_the_sequence_to_be_replaced and _swap_byte_offset > 127) {
        _swap_byte_offset -= 127;
        next_swap_byte_is_place_of_the_replaced_sequence = true;
    } else {
        next_swap_byte_is_place_of_the_replaced_sequence = false;
    }
    _service_byte_offset = _cfg.depth;
    return _swap_byte_offset == 0 or _swap_byte_offset > _cfg.depth ? READ_ERROR : READ_OK;
}

void COBSReader::_reset() {
    _fsm_state = SERVICE_BYTE;
    _service_byte_offset = _cfg.depth;
}

COBSReader::COBSReader() {
    _service_byte_offset = _cfg.depth;
    COBSReader::set_config({.delimiter = '\0', .depth = 255});
}

set_config_status COBSReader::set_config(cobs_config_t config) {
    set_config_status status = SET_OK;
    _is_ready = false;

    if (_fsm_state not_eq SERVICE_BYTE or _service_byte_offset not_eq _cfg.depth) {
        return ERROR_PROCESS_NOT_FINISHED;
    }

    if (config.size_of_the_sequence_to_be_replaced == 1) {
        return ERROR_SIZE_SR;
    }

    if (config.size_of_the_sequence_to_be_replaced and config.depth > 127) {
        status = WARNING_DEPTH_SR;
        config.depth = 127;
    }

    if (config.depth < MIN_BDSP_COBS_DEPTH) {
        status = WARNING_COBS_DEPTH;
        config.depth = MIN_BDSP_COBS_DEPTH;
    }

    _cfg = config;
    _is_ready = true;
    _reset();
    return status;
}


read_status_t COBSZPEReader::_set_swap_byte_offset(uint8_t offset) {
    // Substitution of swap_byte_offset if the delimiter in the configuration is not equal to 0.
    _swap_byte_offset = _cfg.delimiter not_eq 0x00 and offset == 0x00 ? _cfg.delimiter : offset;
    if (_swap_byte_offset > _cfg.depth) {
        _swap_byte_offset -= _cfg.depth;
        next_swap_byte_is_place_of_the_replaced_sequence = true;
    } else {
        next_swap_byte_is_place_of_the_replaced_sequence = false;
    }
    _service_byte_offset = _cfg.depth;
    return _swap_byte_offset == 0 or _swap_byte_offset > _cfg.depth ? READ_ERROR : READ_OK;
}


COBSZPEReader::COBSZPEReader() {
    set_config({'\0', 224, 2,'\0'});
}


set_config_status COBSZPEReader::set_config(cobs_config_t config) {
    set_config_status status = SET_OK;
    _is_ready = false;

    if (_fsm_state not_eq SERVICE_BYTE or _service_byte_offset not_eq _cfg.depth) {
        return ERROR_PROCESS_NOT_FINISHED;
    }

    if (config.size_of_the_sequence_to_be_replaced < 2) {
        return ERROR_SIZE_SR_ZPE;
    }

    if (config.size_of_the_sequence_to_be_replaced and config.depth not_eq 224) {
        return WARNING_DEPTH_ZPE;
    }

    _cfg = config;
    _is_ready = true;
    _reset();
    return status;
}
