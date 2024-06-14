#ifndef BDSP_COBS_WRITER_H
#define BDSP_COBS_WRITER_H

#include "cobs.h"

using namespace cobs_namespace;


class COBSEncoder {
public:
    explicit COBSEncoder(cobs_config_t config, write_handler_t write_handler);
    ~COBSEncoder();
    cobs_encoder_status get_status();
    void reset();
    void send_segment(uint8_t *data_ptr, size_t size);
    cobs_encoder_status finish_sending(bool is_send_with_delimiter = true);
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
