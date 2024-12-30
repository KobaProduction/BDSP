#ifndef BDSP_STREAMS_PPP_ENCODER_H
#define BDSP_STREAMS_PPP_ENCODER_H

#include <cstdint>
#include "BDSP/streams/abstract/encoder.h"

namespace BDSP::streams::PPP {

    class PPPEncoder final : public ABS::AbstractEncoder {
    protected:
        uint8_t _escape_byte = 0x7D;
        uint8_t _end_byte = 0x7E;
        uint8_t _escape_mask = 0x20;

        void _encode(uint8_t byte) override;

        void _finish_encode() override;
    };
}

#endif //BDSP_STREAMS_PPP_ENCODER_H
