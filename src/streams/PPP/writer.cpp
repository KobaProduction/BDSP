#include <BDSP/streams/PPP/writer.h>

using namespace BDSP::streams::PPP;

void PPPWriter::_process_byte(uint8_t byte) {
    if (byte not_eq _end_byte and byte not_eq _escape_byte) return _write(byte);
    _write(_escape_byte);
    _write(byte ^ _escape_mask);
}

void PPPWriter::_finish() {
    _write(_end_byte);
}
