#ifndef BDSP_STREAMS_PPP_DECODER_H
#define BDSP_STREAMS_PPP_DECODER_H

#include <cstdint>
#include "BDSP/streams/abstract/decoder.h"

namespace BDSP::streams::PPP {

    class PPPDecoder final : public ABS::AbstractDecoder {
    private:
        void _reset() override;
    protected:
        uint8_t _escape_byte = 0x7D;
        uint8_t _end_byte = 0x7E;
        uint8_t _escape_mask = 0x20;
        bool _is_escape_state = false;

        streams::decode_status_t _decode(uint8_t byte) override;
    };
}
#endif //BDSP_STREAMS_PPP_DECODER_H
