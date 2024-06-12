#include "reader.h"

COBSReader::COBSReader(cobs_config_t config, cobs_reader_data_callback_t callback) {
    cfg = config;
    data_callback = callback;
    reset();
}

void COBSReader::reset() {
    fsm_state = SERVICE_BYTE;
    swap_byte_offset = 0;
    service_byte_offset = cfg.depth;
}

void COBSReader::parse(uint8_t character) {
    switch (fsm_state) {
        case SERVICE_BYTE:
            if (character == cfg.delimiter) return reset();
            set_swap_byte_offset(character);
            break;
        case REGULAR_BYTE:
            if (character == cfg.delimiter) {
                // Decoding error. The separator should not appear in regular data.
                return set_error_state(character);
            }
            data_callback(character, READ_OK);
            break;
        case SWAP_BYTE:
            if (character == cfg.delimiter) {
                data_callback(character, READ_END);
                return reset();
            }
            set_swap_byte_offset(character);
            data_callback(character, READ_OK);
            break;
        case WAIT_SEPARATOR:
            if (character == cfg.delimiter) return reset();
            break;
    }
    if (fsm_state == WAIT_SEPARATOR) return;
    service_byte_offset--;
    swap_byte_offset--;
    if (service_byte_offset == 0) {
        fsm_state = SERVICE_BYTE;
        service_byte_offset = cfg.depth;
    } else if (swap_byte_offset == 0) {
        fsm_state = SWAP_BYTE;
    } else {
        fsm_state = REGULAR_BYTE;
    }
}

void COBSReader::parse(uint8_t *data_ptr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        parse(data_ptr[i]);
    }
}

void COBSReader::set_swap_byte_offset(uint8_t offset) {
    // Decoding error. The size of the new swap_byte_offset cannot exceed the swap_byte_offset from the configuration.
    if (offset > cfg.depth) {
        set_error_state(offset);
    }
    // Substitution of swap_byte_offset if the delimiter in the configuration is not equal to 0.
    if (not offset and cfg.delimiter) {
        swap_byte_offset = cfg.delimiter;
    } else {
        swap_byte_offset = offset;
    }
}

void COBSReader::set_error_state(uint8_t character) {
    data_callback(character, READ_ERROR);
    fsm_state = WAIT_SEPARATOR;
}
