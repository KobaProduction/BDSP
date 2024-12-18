#ifndef BDSP_DECODERS_TYPES_H
#define BDSP_DECODERS_TYPES_H

#include "stdint.h"

namespace BDSP::decoders {
    enum decode_status_t {
        DECODE_OK, DECODE_END, DECODE_ERROR, UNKNOWN_DECODER_ERROR
    };

    typedef void (*data_handler_t)(uint8_t byte, decode_status_t status, void *data_handler_context_ptr);
}

#endif //BDSP_DECODERS_TYPES_H
