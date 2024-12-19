#include "BDSP/decoders/PPP.h"

using namespace BDSP::decoders::PPP;

void PPPDecoder::_reset() {
    _is_escape_state = false;
}

decode_status_t PPPDecoder::_decode(uint8_t byte) {
    decode_status_t status = DECODE_OK;

    if (byte == _end_byte) {
        return _is_escape_state ? DECODE_ERROR : DECODE_END;
    }

    if (_is_escape_state) {
        _is_escape_state = false;
        byte ^= _escape_mask;
        if (byte not_eq _escape_byte and byte not_eq _end_byte) {
            return DECODE_ERROR;
        }
    } else if (byte == _escape_byte) {
        _is_escape_state = true;
        return DECODE_OK;
    }
    _handler(byte, status);
    return status;
}
