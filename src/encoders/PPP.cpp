#include "BDSP/encoders/PPP.h"

using namespace BDSP::encoders::PPP;

void PPPEncoder::_encode(uint8_t byte) {
    if (byte == 0x7E) {
        _write(0x7D);
        _write(0x5E);
    } else if (byte == 0x7D) {
        _write(0x7D);
        _write(0x5D);
    } else {
        _write(byte);
    }
}

encode_status_t PPPEncoder::finish_encode() {
    _write(0x7E);
    return ENCODE_FINISH;
}
