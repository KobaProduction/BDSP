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

namespace cobs_namespace {

    struct cobs_config_t {
        uint8_t delimiter;
        uint8_t depth;
    };

    enum cobs_encoder_status {
        COBS_OK,
        COBS_BUFFER_MISSING, // Buffer missing. There may be insufficient RAM.
        COBS_EMPTY_DATA,
    };

    enum cobs_fsm_state {SERVICE_BYTE, REGULAR_BYTE, SWAP_BYTE, WAIT_DELIMITER};
    enum cobs_read_state {OK, END, ERROR};

    typedef void (*cobs_reader_data_callback_t)(uint8_t character, cobs_read_state read_state, void *callback_context);
    typedef void (*write_handler_t)(uint8_t *data_ptr, size_t size);
}

#endif //BDSP_COBS_CONFIGURATION_H
