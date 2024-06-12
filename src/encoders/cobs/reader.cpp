#include "./reader.h"

COBSReader::COBSReader(cobs_config_t config_, callback_t callback_) {
    config = config_;
    callback = callback_;
    reset();
}

void COBSReader::reset() {
    state = SERVICE_BYTE;
    swap_byte_offset = 0;
    service_byte_offset = config.depth;
}

void COBSReader::parse(uint8_t character) {
    switch (state) {
        case SERVICE_BYTE:
            if (character == config.delimiter) return reset();
            set_swap_byte_offset(character);
            break;
        case REGULAR_BYTE:
            if (character == config.delimiter) {
                // Decoding error. The separator should not appear in regular data.
                return set_error_state(character);
            }
            callback(character, READ_OK);
            break;
        case SWAP_BYTE:
            if (character == config.delimiter) {
                callback(character, READ_END);
                return reset();
            }
            set_swap_byte_offset(character);
            callback(character, READ_OK);
            break;
        case WAIT_SEPARATOR:
            if (character == config.delimiter) reset();
            break;
    }
    if (state == WAIT_SEPARATOR) return;
    service_byte_offset--;
    swap_byte_offset--;
    if (service_byte_offset == 0) {
        state = SERVICE_BYTE;
        service_byte_offset = config.depth;
    } else if (swap_byte_offset == 0){
        state = SWAP_BYTE;
    } else {
        state = REGULAR_BYTE;
    }
}

void COBSReader::parse(uint8_t *characters, size_t size) {
    for (int i = 0; i < size; ++i) {
        parse(characters[i]);
    }
}

void COBSReader::set_swap_byte_offset(uint8_t offset) {
    if (offset > config.depth) {
        // Decoding error. The size of the new swap_byte_offset cannot exceed the swap_byte_offset from the configuration.
        set_error_state(offset);
    }
    // Substitution of swap_byte_offset if the delimiter in the configuration is not equal to 0.
    if (not offset and config.delimiter) {
        swap_byte_offset = config.delimiter;
    } else {
        swap_byte_offset = offset;
    }
}

void COBSReader::set_error_state(uint8_t character = 0) {
    callback(character, READ_ERROR);
    state = WAIT_SEPARATOR;
}
