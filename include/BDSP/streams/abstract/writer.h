#ifndef BDSP_STREAMS_ABSTRACT_ENCODER_H
#define BDSP_STREAMS_ABSTRACT_ENCODER_H

#include <stdint.h>

#include "BDSP/streams/types.h"

namespace BDSP::streams::ABS {

    class AbstractWriter : public IWriter {
        stream_writer_t _writer = nullptr;
        void *_writer_context = nullptr;
    protected:
        bool _is_ready = true;

        virtual void _process_byte(uint8_t byte) = 0;

        virtual void _finish() = 0;

        void _write(uint8_t byte);

        void _write(uint8_t *buffer_ptr, size_t size);

    public:
        write_status_t write(uint8_t byte) final;

        write_status_t write(uint8_t *buffer_ptr, size_t size) final;

        write_status_t finish() final;

        void set_stream_writer(stream_writer_t writer, void *context_ptr) final;
    };
}

#endif //BDSP_STREAMS_ABSTRACT_ENCODER_H
