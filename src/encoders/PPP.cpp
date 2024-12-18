#include "BDSP/encoders/PPP.h"

using namespace BDSP::encoders::PPP;

void PPPEncoder::_encode(uint8_t byte) {
    if (byte not_eq _end_byte and byte not_eq _escape_byte) return _write(byte);
    _write(_escape_byte);
    _write(byte ^ _escape_mask);
}

encode_status_t PPPEncoder::finish_encode() {
    _write(_end_byte);
    return ENCODE_FINISH;
}
