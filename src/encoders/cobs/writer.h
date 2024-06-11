#ifndef BDSP_WRITER_H
#define BDSP_WRITER_H

#include "./config.h"

enum cobs_writer_status_t {
    COBS_OK,
    COBS_BUFFER_MISSING, // Buffer missing. There may be insufficient RAM.
    COBS_EMPTY_DATA,
};

class COBSWriter {
public:
    explicit COBSWriter(cobs_config_t config, void (*writer_ptr)(uint8_t *data_ptr, size_t length));
    ~COBSWriter();
    cobs_writer_status_t get_status();
    void reset();
    void send_segment(uint8_t *data_ptr, size_t length);
    cobs_writer_status_t finish_sending(bool is_send_with_delimiter = true);
private:
    cobs_config_t config = {DEFAULT_COBS_DELIMITER, DEFAULT_COBS_DEPTH};
    void (*writer_h) (uint8_t*, size_t) = nullptr;
    uint8_t *buffer_ptr = nullptr;
    uint8_t offset = 1;
    uint8_t *current_buffer_ptr = nullptr;
    uint8_t *offset_place_ptr = nullptr;
    void write_offset(uint8_t new_offset);
};

#endif //BDSP_WRITER_H
