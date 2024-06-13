#ifndef BDSP_COBS_READER_H
#define BDSP_COBS_READER_H

#include "cobs.h"

using namespace cobs_namespace;


class COBSDecoder {
public:
    explicit COBSDecoder(cobs_config_t config, cobs_reader_data_callback_t callback, void *callback_context = nullptr);
    void reset(bool is_wait_delimiter = false);
    void parse(uint8_t character);
    void parse(uint8_t *data_ptr, size_t size);
private:
    cobs_config_t cfg {DEFAULT_COBS_DELIMITER, DEFAULT_COBS_DEPTH};
    cobs_reader_data_callback_t data_callback = nullptr;
    void *data_callback_context = nullptr;
    cobs_fsm_state fsm_state = SERVICE_BYTE;
    uint8_t swap_byte_offset = 0;
    uint8_t service_byte_offset = 0;
    void set_swap_byte_offset(uint8_t offset);
    void set_error_state(uint8_t character);
};

#endif //BDSP_COBS_READER_H