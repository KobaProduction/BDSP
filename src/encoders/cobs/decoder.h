#ifndef BDSP_COBS_READER_H
#define BDSP_COBS_READER_H

#include "cobs.h"

using namespace cobs;


class COBSDecoder {
public:
    explicit COBSDecoder(cobs_config_t config, cobs_decoder_data_callback_t data_callback, void *data_callback_context_ptr = nullptr);
    void reset(bool is_wait_delimiter = false);
    void parse(uint8_t byte);
    void parse(uint8_t *buffer_ptr, size_t size);
private:
    cobs_config_t _cfg {DEFAULT_COBS_DELIMITER, DEFAULT_COBS_DEPTH};
    cobs_decoder_data_callback_t _data_callback = nullptr;
    void *_callback_context_ptr = nullptr;
    fsm_state_t _fsm_state = SERVICE_BYTE;
    uint8_t _swap_byte_offset = 0;
    uint8_t _service_byte_offset = 0;
    void _set_swap_byte_offset(uint8_t offset);
    void _set_error_state(uint8_t byte);
};

#endif //BDSP_COBS_READER_H