#ifndef BDSP_INTERFACE_H
#define BDSP_INTERFACE_H

#include "abs_decoder.h"

namespace BDSP::decoders {

    class IDecoder {
    public:
        virtual void set_data_handler(data_handler_t handler, void *context_ptr) = 0;

        virtual void reset_decode_state(bool need_wait_delemiter) = 0;

        virtual decode_status_t decode(uint8_t byte) = 0;

        virtual decode_status_t decode(uint8_t *buffer_ptr, size_t size) = 0;

    };
}

#endif //BDSP_INTERFACE_H
