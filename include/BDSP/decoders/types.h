#ifndef BDSP_DECODERS_TYPES_H
#define BDSP_DECODERS_TYPES_H

#include <cstdint>

namespace BDSP::decoders {
    enum decode_state_t {
        DECODE_OK, DECODE_END, DECODE_ERROR, WAITING_FOR_DELIMITER, DATA_HANDLER_NOT_INSTALLED
    };

    typedef void (*data_handler_t)(uint8_t byte, decode_state_t state, void *data_handler_context_ptr);
}

#endif //BDSP_DECODERS_TYPES_H
