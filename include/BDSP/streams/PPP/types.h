#ifndef BDSP_STREAMS_PPP_TYPES_H
#define BDSP_STREAMS_PPP_TYPES_H

#include <stdint.h>

namespace BDSP::streams::PPP {

struct ppp_config_t {
    uint8_t end_byte = 0x7E;
    uint8_t escape_byte = 0x7D;
    uint8_t escape_mask = 0x20;
};

enum set_ppp_config_status {
    SET_OK = 0,
    ERROR_PROCESS_NOT_FINISHED,
    ERROR_INVERTED_END_AND_ESCAPE_BYTES_IS_EQUALS,
    ERROR_END_AND_ESCAPE_BYTES_IS_EQUALS,
    ERROR_ESCAPE_MASK_IS_NULL,
};

} // namespace BDSP::streams::PPP

#endif // BDSP_STREAMS_PPP_TYPES_H
