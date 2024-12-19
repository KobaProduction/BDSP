#ifndef BDSP_ENCODERS_TYPES_H
#define BDSP_ENCODERS_TYPES_H

#include <cstdint>

namespace BDSP::encoders {
    typedef void (*write_handler_t)(uint8_t byte, void *write_handler_context_ptr);

    enum encode_status_t {
        ENCODE_OK, ENCODE_FINISH, UNKNOWN_ENCODER_ERROR
    };
}

#endif //BDSP_ENCODERS_TYPES_H
