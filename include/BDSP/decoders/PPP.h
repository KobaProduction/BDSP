#ifndef BDSP_DECODERS_PPP_H
#define BDSP_DECODERS_PPP_H

#include <cstdint>
#include "BDSP/decoders/abs_decoder.h"

namespace BDSP::decoders::PPP {
    using namespace BDSP::decoders;


    class PPPDecoder final : public ABS::AbstractDecoder {
    protected:
        bool is_last_7D = false;

        decode_status_t _decode(uint8_t byte) override;
    };
}
#endif //BDSP_DECODERS_PPP_H
