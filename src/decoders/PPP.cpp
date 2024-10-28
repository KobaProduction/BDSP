#include "BDSP/decoders/PPP.h"

using namespace BDSP::decoders::PPP;

decode_state_t PPPDecoder::_decode(uint8_t byte) {
    decode_state_t status = DECODE_OK;
    switch (byte) {
        case 0x7E:
            status = DECODE_END;
            break;
        case 0x7D:
            if (is_last_7D) status = DECODE_ERROR;
            else is_last_7D = true;
            break;
        case 0x5D:
            if (not is_last_7D) break;
            byte = 0x7D;
            is_last_7D = false;
            break;
        case 0x5E:
            if (not is_last_7D) break;
            byte = 0x7E;
            is_last_7D = false;
            break;
        default:
            break;
    }
    if (byte == 0x7D and is_last_7D) return status;
    _handler(byte, status);
    return status;
}
