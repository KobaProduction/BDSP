#include "BDSP/decoders/PPP.h"

using namespace BDSP::decoders::PPP;

decode_status_t PPPDecoder::_decode(uint8_t byte) {
    decode_status_t status = DECODE_OK;
    if (is_last_7D) {
        if (byte == 0x5D) {
            byte = 0x7D;
        } else if (byte == 0x5E) {
            byte = 0x7E;
        } else {
            status = DECODE_ERROR;
        }
        is_last_7D = false;
    } else if (byte == 0x7D) {
        is_last_7D = true;
    } else if (byte == 0x7E) {
        status = DECODE_END;
        _is_waiting_delemiter = false;
    }
    if (not _is_waiting_delemiter and not is_last_7D) _handler(byte, status);
    return status;
}
