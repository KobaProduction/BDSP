#include "BDSP/streams/PPP/checkers.h"

using namespace BDSP::streams::PPP;

set_ppp_config_status core::check_ppp_config(ppp_config_t config) {
    if (config.escape_byte == config.end_byte) {
        return ERROR_END_AND_ESCAPE_BYTES_IS_EQUALS;
    }

    if ((config.escape_byte ^ config.escape_mask) == config.end_byte) {
        return ERROR_INVERTED_END_AND_ESCAPE_BYTES_IS_EQUALS;
    }

    if (config.escape_mask == 0x00) {
        return ERROR_ESCAPE_MASK_IS_NULL;
    }

    return SET_OK;
}
