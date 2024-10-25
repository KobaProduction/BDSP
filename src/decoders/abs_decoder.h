#ifndef BDSP_DECODERS_ABS_H
#define BDSP_DECODERS_ABS_H

#include <cstdint>

#include "types.h"

namespace BDSP::decoders::ABS {

    class AbstractDecoder {
    private:
        data_handler_t _data_handler = nullptr;
        void *_data_handler_context = nullptr;
    protected:
        bool is_waiting_delemiter = false;

        void _handler(uint8_t byte, decode_state_t state);

        virtual decode_state_t _decode(uint8_t byte) = 0;

    public:
        void set_data_handler(data_handler_t handler, void *context_ptr = nullptr);

        decode_state_t decode(uint8_t byte);

        [[maybe_unused]] decode_state_t decode(uint8_t *buffer_ptr, size_t size);

    };
}

#endif //BDSP_DECODERS_ABS_H
