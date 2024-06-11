#include "./writer.h"

COBSWriter::COBSWriter(cobs_config_t config_, void (*writer_ptr)(uint8_t *data_ptr, size_t length)) {
    config = config_;
    writer_h = writer_ptr;
    if (not config.depth) config.depth = DEFAULT_COBS_DEPTH;
    buffer_ptr = reinterpret_cast<uint8_t*>(malloc(config.depth));
    reset();
}

COBSWriter::~COBSWriter() {
    finish_sending();
    free(buffer_ptr);
}

cobs_writer_status_t COBSWriter::get_status() {
    if (not buffer_ptr) return cobs_writer_status_t::COBS_BUFFER_MISSING;
    return COBS_OK;
}

void COBSWriter::reset() {
    offset = 1;
    current_buffer_ptr = buffer_ptr;
    offset_place_ptr = current_buffer_ptr++;
}

cobs_writer_status_t COBSWriter::finish_sending(bool is_send_with_delimiter) {
    uint8_t size = current_buffer_ptr - buffer_ptr;
    if (size < 2) return cobs_writer_status_t::COBS_EMPTY_DATA;
    writer_h(buffer_ptr, size);
    if (is_send_with_delimiter) writer_h(&config.delimiter, 1);
    reset();
    return cobs_writer_status_t::COBS_OK;
}

void COBSWriter::send_segment(uint8_t *data_ptr, size_t length) {
    uint8_t *current_byte_ptr = data_ptr;
    for (size_t i = 0; i < length; ++i) {
        uint8_t character = *current_byte_ptr++;
        if (character != config.delimiter and current_buffer_ptr - buffer_ptr < config.depth) {
            *current_buffer_ptr++ = character;
            offset++;
        } else {
            if (current_buffer_ptr - buffer_ptr == config.depth) {
                current_byte_ptr--; i--;
            }
            write_offset(offset);
            writer_h(buffer_ptr, current_buffer_ptr - buffer_ptr);
            reset();
        }
    }
    write_offset(current_buffer_ptr - offset_place_ptr);
}

void COBSWriter::write_offset(uint8_t new_offset) {
    // Substituting the offset value if it is equal to the delimiter.
    if (offset == config.delimiter and config.delimiter != 0) *offset_place_ptr = 0;
    else *offset_place_ptr = new_offset;
}
