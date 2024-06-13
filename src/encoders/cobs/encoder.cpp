#include "encoder.h"

COBSEncoder::COBSEncoder(cobs_config_t config, write_handler_t handler) {
    cfg = config;
    write_handler = handler;
    if (not cfg.depth) cfg.depth = DEFAULT_COBS_DEPTH;
    buffer_ptr = reinterpret_cast<uint8_t*>(malloc(cfg.depth));
    reset();
}

COBSEncoder::~COBSEncoder() {
    finish_sending();
    free(buffer_ptr);
}

cobs_encoder_status COBSEncoder::get_status() {
    if (not buffer_ptr) return COBS_BUFFER_MISSING;
    return COBS_OK;
}

void COBSEncoder::reset() {
    offset = 1;
    current_buffer_ptr = buffer_ptr;
    offset_place_ptr = current_buffer_ptr++;
}

cobs_encoder_status COBSEncoder::finish_sending(bool is_send_with_delimiter) {
    uint8_t size = current_buffer_ptr - buffer_ptr;
    if (size < 2) return COBS_EMPTY_DATA;
    write_handler(buffer_ptr, size);
    if (is_send_with_delimiter) write_handler(&cfg.delimiter, 1);
    reset();
    return COBS_OK;
}

void COBSEncoder::send_segment(uint8_t *data_ptr, size_t size) {
    uint8_t *current_byte_ptr = data_ptr;
    for (size_t i = 0; i < size; ++i) {
        uint8_t character = *current_byte_ptr++;
        if (character != cfg.delimiter and current_buffer_ptr - buffer_ptr < cfg.depth) {
            *current_buffer_ptr++ = character;
            offset++;
        } else {
            if (current_buffer_ptr - buffer_ptr == cfg.depth) {
                current_byte_ptr--; i--;
            }
            set_offset(offset);
            write_handler(buffer_ptr, current_buffer_ptr - buffer_ptr);
            reset();
        }
    }
    set_offset(current_buffer_ptr - offset_place_ptr);
}

void COBSEncoder::set_offset(uint8_t new_offset) {
    // Substituting the offset value if it is equal to the delimiter.
    if (offset == cfg.delimiter and cfg.delimiter != 0) *offset_place_ptr = 0;
    else *offset_place_ptr = new_offset;
}
