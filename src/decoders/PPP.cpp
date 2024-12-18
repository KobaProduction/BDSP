#include "BDSP/decoders/PPP.h"

using namespace BDSP::decoders::PPP;

decode_status_t PPPDecoder::_decode(uint8_t byte) {
    decode_status_t status = DECODE_OK;
    if (_is_escape_state) {
        _is_escape_state = false;
        byte ^= _escape_mask;
        if (byte not_eq _escape_byte and byte not_eq _end_byte) status = DECODE_ERROR;
    } else if (byte == _escape_byte) {
        _is_escape_state = true;
    } else if (byte == _end_byte) {
        status = DECODE_END;
        _is_waiting_delemiter = false;
    }
    if (not _is_escape_state) _handler(byte, status);
    return status;
}
