#ifndef BDSP_COBS_H
#define BDSP_COBS_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define DEFAULT_COBS_DEPTH 255
#define DEFAULT_COBS_DELIMITER '\0'

#ifndef MIN_COBS_DEPTH
#define MIN_COBS_DEPTH 16
#endif


namespace COBS {

    struct config_t {
        uint8_t delimiter;
        uint8_t depth;
    };

    enum encoder_status_t {
        COBS_OK,
        COBS_BUFFER_MISSING, // Buffer missing. There may be insufficient RAM.
        COBS_EMPTY_DATA,
    };

    enum fsm_state_t {SERVICE_BYTE, REGULAR_BYTE, SWAP_BYTE, WAIT_DELIMITER};
    enum decode_state_t {DECODE_OK, DECODE_END, DECODE_ERROR};

    typedef void (*decoder_data_callback_t)(uint8_t byte, decode_state_t decode_state, void *callback_context_ptr);
    typedef void (*write_handler_t)(uint8_t *buffer_ptr, size_t size, void *write_handler_context_ptr);
}

#endif //BDSP_COBS_H