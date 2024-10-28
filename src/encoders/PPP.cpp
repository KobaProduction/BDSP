#include "BDSP/encoders/PPP.h"

using namespace BDSP::encoders::PPP;

void PPPEncoder::_write_delimiter() {
    _writer(0x7E, _writer_context);
}

void PPPEncoder::_encode(uint8_t byte) {
    if (byte == 0x7E) {
        _writer(0x7D, _writer_context);
        _writer(0x5E, _writer_context);
    } else if (byte == 0x7D) {
        _writer(0x7D, _writer_context);
        _writer(0x5D, _writer_context);
    } else {
        _writer(byte, _writer_context);
    }
}
