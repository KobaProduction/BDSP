#ifndef BDSP_ENCODERS_ABS_H
#define BDSP_ENCODERS_ABS_H

#include <cstdint>

#include <BDSP/encoders/types.h>
#include <BDSP/encoders/interface.h>

namespace BDSP::encoders::ABS {
    using namespace BDSP::encoders;

    class AbstractEncoder : public IEncoder {
        write_handler_t _writer = nullptr;
        void *_writer_context = nullptr;
    protected:
        bool _is_ready = true;

        virtual void _encode(uint8_t byte) = 0;

        virtual void _finish_encode() = 0;

        void _write(uint8_t byte);

        void _write(uint8_t *buffer_ptr, size_t size);

    public:
        encode_status_t encode(uint8_t byte) final;

        encode_status_t encode(uint8_t *buffer_ptr, size_t size) final;

        encode_status_t finish_encode() final;

        void set_writer(write_handler_t writer, void *context_ptr) final;
    };
}

#endif //BDSP_ENCODERS_ABS_H
