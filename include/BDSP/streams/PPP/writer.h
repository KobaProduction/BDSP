#ifndef BDSP_STREAMS_PPP_WRITER_H
#define BDSP_STREAMS_PPP_WRITER_H

#include <stdint.h>

#include "BDSP/streams/abstract/writer.h"

namespace BDSP::streams::PPP {

    class PPPWriter final : public ABS::AbstractWriter {
    protected:
        uint8_t _escape_byte = 0x7D;
        uint8_t _end_byte = 0x7E;
        uint8_t _escape_mask = 0x20;

        void _process_byte(uint8_t byte) override;

        void _finish() override;
    };
}

#endif // BDSP_STREAMS_PPP_WRITER_H
