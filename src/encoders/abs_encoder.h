#ifndef BDSP_ENCODERS_ABS_H
#define BDSP_ENCODERS_ABS_H

#include <cstdint>

#include "./types.h"

namespace BDSP::encoders::ABS {
    using namespace BDSP::encoders;

    class AbstractEncoder {
    protected:
        write_handler_t _writer = nullptr;
        void *_writer_context = nullptr;

        virtual void _encode(uint8_t byte) = 0;

        virtual void _write_delimiter() = 0;

    public:
        void set_writer(write_handler_t writer, void *context_ptr = nullptr);

        encode_status_t encode(uint8_t byte);

        [[maybe_unused]] encode_status_t encode(uint8_t *buffer_ptr, size_t size);
    };
}

#endif //BDSP_ENCODERS_ABS_H
