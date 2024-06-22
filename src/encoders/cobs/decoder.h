#ifndef BDSP_COBS_READER_H
#define BDSP_COBS_READER_H

#include "cobs.h"


class COBSDecoder {
public:
    explicit COBSDecoder(COBS::config_t config, COBS::decoder_data_callback_t data_callback, void *data_callback_context_ptr = nullptr);
    void reset(bool is_wait_delimiter = false);
    void parse(uint8_t byte);
    void parse(uint8_t *buffer_ptr, size_t size);
private:
    COBS::config_t _cfg {DEFAULT_COBS_DELIMITER, DEFAULT_COBS_DEPTH};
    COBS::decoder_data_callback_t _data_callback = nullptr;
    void *_callback_context_ptr = nullptr;
    COBS::fsm_state_t _fsm_state = COBS::SERVICE_BYTE;
    uint8_t _swap_byte_offset = 0;
    uint8_t _service_byte_offset = 0;
    void _set_swap_byte_offset(uint8_t offset);
    void _set_error_state(uint8_t byte);
};

#endif //BDSP_COBS_READER_H