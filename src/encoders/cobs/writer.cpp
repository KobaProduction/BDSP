#include <./encoders/cobs/writer.h>

COBSWriter::COBSWriter(cobs_config_t config_, void (*writer_ptr)(uint8_t *, size_t)) {
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
    code = 1;
    dst = buffer_ptr;
    code_ptr = dst++;
}

cobs_writer_status_t COBSWriter::finish_sending(bool is_send_with_delimiter) {
    uint8_t size = dst - buffer_ptr;
    if (size < 2) return cobs_writer_status_t::COBS_EMPTY_DATA;
    writer_h(buffer_ptr, size);
    if (is_send_with_delimiter) writer_h(&config.delimiter, 1);
    reset();
    return cobs_writer_status_t::COBS_OK;
}

void COBSWriter::send_segment(uint8_t *data_ptr, size_t size) {
    uint8_t *ptr = data_ptr;
    for (uint16_t i = 0; i < size; ++i) {
        uint8_t character = *ptr++;
        if (character != config.delimiter && dst - buffer_ptr < config.depth) {
            *dst++ = character;
            code++;
        } else {
            if (dst - buffer_ptr == config.depth) { ptr--; i--;};
            if (config.delimiter != 0 && code == config.delimiter) code = 0;
            *code_ptr = code;
            writer_h(buffer_ptr, dst - buffer_ptr);
            reset();
        }
    }
    *code_ptr = dst - code_ptr;
}
