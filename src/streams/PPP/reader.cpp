#include "BDSP/streams/PPP/reader.h"

using namespace BDSP::streams;
using namespace BDSP::streams::PPP;

void PPPDecoder::_reset() {
    _is_escape_state = false;
}

read_status_t PPPDecoder::_process_byte(uint8_t byte) {
    read_status_t status = READ_OK;

    if (byte == _end_byte) {
        return _is_escape_state ? READ_ERROR : READ_END;
    }

    if (_is_escape_state) {
        _is_escape_state = false;
        byte ^= _escape_mask;
        if (byte not_eq _escape_byte and byte not_eq _end_byte) {
            return READ_ERROR;
        }
    } else if (byte == _escape_byte) {
        _is_escape_state = true;
        return READ_OK;
    }
    _handler(byte, status);
    return status;
}
