#ifndef BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_TYPES_H
#define BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_TYPES_H

#include <stdint.h>

namespace BDSP::streams::strategies::byte_stuffing {

struct byte_stuffing_config_t {
    uint8_t delimiter_byte = 0xFE;
    uint8_t escape_byte = 0xFD;
    uint8_t escape_mask = 0x20;
};

enum set_byte_stuffing_config_status {
    SET_OK = 0,
    ERROR_PROCESS_NOT_FINISHED,
    ERROR_INVERTED_END_AND_ESCAPE_BYTES_IS_EQUALS,
    ERROR_END_AND_ESCAPE_BYTES_IS_EQUALS,
    ERROR_ESCAPE_MASK_IS_NULL,
};

} // namespace BDSP::streams::strategies::byte_stuffing

#endif // BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_TYPES_H
