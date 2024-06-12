#ifndef BDSP_COBS_CONFIGURATION_H
#define BDSP_COBS_CONFIGURATION_H

#ifdef ARDUINO
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#else
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#endif

#define DEFAULT_COBS_DEPTH 255
#define DEFAULT_COBS_DELIMITER '\0'

struct cobs_config_t {
    uint8_t delimiter;
    uint8_t depth;
};

enum cobs_fsm_state {SERVICE_BYTE, REGULAR_BYTE, SWAP_BYTE, WAIT_SEPARATOR};
enum cobs_read_state {READ_OK, READ_END, READ_ERROR};

typedef void (*cobs_reader_data_callback_t)(uint8_t character, cobs_read_state read_state);

enum cobs_writer_status_t {
    COBS_OK,
    COBS_BUFFER_MISSING, // Buffer missing. There may be insufficient RAM.
    COBS_EMPTY_DATA,
};

#endif //BDSP_COBS_CONFIGURATION_H
