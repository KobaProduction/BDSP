#ifndef BDSP_DECODERS_PPP_H
#define BDSP_DECODERS_PPP_H

#include <cstdint>
#include "BDSP/decoders/abs_decoder.h"

namespace BDSP::decoders::PPP {
    using namespace BDSP::decoders;


    class PPPDecoder final : public ABS::AbstractDecoder {
    protected:
        uint8_t _escape_byte = 0x7D;
        uint8_t _end_byte = 0x7E;
        uint8_t _escape_mask = 0x20;
        bool _is_escape_state = false;

        decode_status_t _decode(uint8_t byte) override;
    };
}
#endif //BDSP_DECODERS_PPP_H
