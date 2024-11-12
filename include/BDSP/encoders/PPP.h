#ifndef BDSP_ENCODERS_PPP_H
#define BDSP_ENCODERS_PPP_H

#include <cstdint>
#include "BDSP/encoders/abs_encoder.h"

namespace BDSP::encoders::PPP {
    using namespace BDSP::encoders;

    class PPPEncoder final : public ABS::AbstractEncoder {
        void _encode(uint8_t byte) override;

    public:
        encode_status_t finish_encode() override;
    };
}

#endif //BDSP_ENCODERS_PPP_H
