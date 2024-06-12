#ifndef BDSP_COBS_WRITER_H
#define BDSP_COBS_WRITER_H

#include "configuration.h"


class COBSWriter {
public:
    explicit COBSWriter(cobs_config_t config, void (*writer_ptr)(uint8_t *data_ptr, size_t size));
    ~COBSWriter();
    cobs_writer_status_t get_status();
    void reset();
    void send_segment(uint8_t *data_ptr, size_t size);
    cobs_writer_status_t finish_sending(bool is_send_with_delimiter = true);
private:
    cobs_config_t cfg = {DEFAULT_COBS_DELIMITER, DEFAULT_COBS_DEPTH};
    void (*write_handler) (uint8_t *data_ptr, size_t size) = nullptr;
    uint8_t *buffer_ptr = nullptr;
    uint8_t offset = 1;
    uint8_t *current_buffer_ptr = nullptr;
    uint8_t *offset_place_ptr = nullptr;
    void set_offset(uint8_t new_offset);
};

#endif //BDSP_COBS_WRITER_H
