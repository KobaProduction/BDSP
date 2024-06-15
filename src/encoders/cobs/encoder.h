#ifndef BDSP_COBS_WRITER_H
#define BDSP_COBS_WRITER_H

#include "cobs.h"

using namespace cobs;


class COBSEncoder {
public:
    explicit COBSEncoder(cobs_config_t config, cobs_write_handler_t handler, void *handler_context_ptr = nullptr);
    ~COBSEncoder();
    cobs_encoder_status_t get_status();
    cobs_encoder_status_t finish_sending(bool is_send_with_delimiter = true);
    void                send_segment(uint8_t *buffer_ptr, size_t size);
    void                reset();
private:
    cobs_config_t _cfg = {DEFAULT_COBS_DELIMITER, DEFAULT_COBS_DEPTH};
    cobs_write_handler_t _write_handler = nullptr;
    void *_handler_context_ptr = nullptr;
    uint8_t *_buffer_ptr = nullptr;
    uint8_t *_current_buffer_ptr = nullptr;
    uint8_t _service_byte_offset = 1;
    uint8_t *_service_byte_ptr = nullptr;
};

#endif //BDSP_COBS_WRITER_H