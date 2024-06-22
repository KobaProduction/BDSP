#ifndef BDSP_COBS_WRITER_H
#define BDSP_COBS_WRITER_H

#include "cobs.h"


class COBSEncoder {
public:
    explicit COBSEncoder(COBS::config_t config, COBS::write_handler_t write_handler, void *write_handler_context_ptr = nullptr);
    ~COBSEncoder();
    COBS::encoder_status_t get_status();
    COBS::encoder_status_t finish_encoding(bool is_send_with_delimiter = true);
    void send_segment(uint8_t *buffer_ptr, size_t size);
    void send_byte(uint8_t &byte);
    void reset();
private:
    COBS::config_t _cfg = {DEFAULT_COBS_DELIMITER, DEFAULT_COBS_DEPTH};
    COBS::write_handler_t _write_handler = nullptr;
    void *_handler_context_ptr = nullptr;
    uint8_t *_buffer_ptr = nullptr;
    uint8_t *_current_buffer_ptr = nullptr;
    uint8_t _service_byte_offset = 1;
    uint8_t *_service_byte_ptr = nullptr;
};

#endif //BDSP_COBS_WRITER_H