#ifndef BDSP_COBS_H
#define BDSP_COBS_H

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
#ifndef MIN_COBS_DEPTH
#define MIN_COBS_DEPTH 10
#endif


namespace cobs {

    struct cobs_config_t {
        uint8_t delimiter;
        uint8_t depth;
    };

    enum cobs_encoder_status_t {
        COBS_OK,
        COBS_BUFFER_MISSING, // Buffer missing. There may be insufficient RAM.
        COBS_EMPTY_DATA,
    };

    enum fsm_state_t {SERVICE_BYTE, REGULAR_BYTE, SWAP_BYTE, WAIT_DELIMITER};
    enum decode_state_t {DECODE_OK, DECODE_END, DECODE_ERROR};

    typedef void (*cobs_decoder_data_callback_t)(uint8_t byte, decode_state_t decode_state, void *callback_context_ptr);
    typedef void (*cobs_write_handler_t)(uint8_t *buffer_ptr, size_t size, void *write_handler_context_ptr);
}

#endif //BDSP_COBS_H