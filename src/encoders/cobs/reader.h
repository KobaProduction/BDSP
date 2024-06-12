#ifndef BDSP_READER_H
#define BDSP_READER_H

#include "./config.h"

enum cobs_fsm_state {SERVICE_BYTE, REGULAR_BYTE, SWAP_BYTE, WAIT_SEPARATOR};
enum cobs_read_state {READ_OK, READ_END, READ_ERROR};

typedef void (*callback_t)(uint8_t character, cobs_read_state read_state);

class COBSReader {
public:
    explicit COBSReader(cobs_config_t config, callback_t callback);
    void reset();
    void parse(uint8_t character);
    void parse(uint8_t *characters, size_t size);
private:
    cobs_config_t config{};
    callback_t callback = nullptr;
    cobs_fsm_state state = SERVICE_BYTE;
    uint8_t swap_byte_offset = 0;
    uint8_t service_byte_offset = 0;
    void set_swap_byte_offset(uint8_t offset);
    void set_error_state(uint8_t character);
};

#endif //BDSP_READER_H
