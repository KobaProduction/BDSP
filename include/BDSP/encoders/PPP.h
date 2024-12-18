#ifndef BDSP_ENCODERS_PPP_H
#define BDSP_ENCODERS_PPP_H

#include <cstdint>
#include "BDSP/encoders/abs_encoder.h"

namespace BDSP::encoders::PPP {
    using namespace BDSP::encoders;

    class PPPEncoder final : public ABS::AbstractEncoder {
    protected:
        uint8_t _escape_byte = 0x7D;
        uint8_t _end_byte = 0x7E;
        uint8_t _escape_mask = 0x20;

        void _encode(uint8_t byte) override;

    public:
        encode_status_t finish_encode() override;
    };
}

#endif //BDSP_ENCODERS_PPP_H
