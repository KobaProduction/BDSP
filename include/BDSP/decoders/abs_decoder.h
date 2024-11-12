#ifndef BDSP_DECODERS_ABS_H
#define BDSP_DECODERS_ABS_H

#include <cstdint>

#include "BDSP/decoders/types.h"

namespace BDSP::decoders::ABS {
    using namespace BDSP::decoders;

    class AbstractDecoder {
    private:
        data_handler_t _data_handler = nullptr;
        void *_data_handler_context = nullptr;
    protected:
        bool _is_waiting_delemiter = false;

        void _handler(uint8_t byte, decode_status_t state);

        virtual decode_status_t _decode(uint8_t byte) = 0;

    public:
        void set_data_handler(data_handler_t handler, void *context_ptr = nullptr);

        decode_status_t decode(uint8_t byte);

        decode_status_t decode(uint8_t *buffer_ptr, size_t size);

    };
}

#endif //BDSP_DECODERS_ABS_H
