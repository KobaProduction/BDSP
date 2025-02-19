#include "BDSP/streams/strategies/byte_stuffing/mixins.h"

using namespace BDSP::streams::strategies::byte_stuffing;
using namespace BDSP::streams::strategies::byte_stuffing::core;

set_byte_stuffing_config_status ByteStuffingConfigsMixin::_check_config(byte_stuffing_config_t config) {
    if (config.escape_byte == config.delimiter_byte) {
        return ERROR_END_AND_ESCAPE_BYTES_IS_EQUALS;
    }
    if ((config.escape_byte ^ config.escape_mask) == config.delimiter_byte) {
        return ERROR_INVERTED_END_AND_ESCAPE_BYTES_IS_EQUALS;
    }
    if (config.escape_mask == 0x00) {
        return ERROR_ESCAPE_MASK_IS_NULL;
    }
    return SET_OK;
}

byte_stuffing_config_t ByteStuffingConfigsMixin::get_default_config() {
    return {.delimiter_byte = 0xFE, .escape_byte = 0xFD, .escape_mask = 0x20};
}

byte_stuffing_config_t PPPConfigsMixin::get_default_config() {
    return {.delimiter_byte = 0x7E, .escape_byte = 0x7D, .escape_mask = 0x20};
}
