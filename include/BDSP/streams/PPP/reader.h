#ifndef BDSP_STREAMS_PPP_READER_H
#define BDSP_STREAMS_PPP_READER_H

#include <stdint.h>

#include "BDSP/streams/abstract/reader.h"

namespace BDSP::streams::PPP {

    class PPPDecoder final : public ABS::AbstractReader {
    private:
        void _reset() override;
    protected:
        uint8_t _escape_byte = 0x7D;
        uint8_t _end_byte = 0x7E;
        uint8_t _escape_mask = 0x20;
        bool _is_escape_state = false;

        streams::read_status_t _process_byte(uint8_t byte) override;
    };
}
#endif // BDSP_STREAMS_PPP_READER_H
