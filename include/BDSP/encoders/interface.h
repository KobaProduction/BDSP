#ifndef BDSP_ENCODERS_ENCODER_INTERFACE_H
#define BDSP_ENCODERS_ENCODER_INTERFACE_H

#include <BDSP/encoders/types.h>

namespace BDSP::encoders {

    class IEncoder {
    public:
        virtual encode_status_t encode(uint8_t byte) = 0;

        virtual encode_status_t encode(uint8_t *buffer_ptr, size_t size) = 0;

        virtual encode_status_t finish_encode() = 0;

        virtual void set_writer(write_handler_t writer, void *context_ptr) = 0;

    };
}

#endif //BDSP_ENCODERS_ENCODER_INTERFACE_H
