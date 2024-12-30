#ifndef BDSP_STREAMS_COBS_TYPES_H
#define BDSP_STREAMS_COBS_TYPES_H

#ifndef MIN_BDSP_COBS_DEPTH
#define MIN_BDSP_COBS_DEPTH 16
#endif

#include <cstdint>

namespace BDSP::streams::COBS {
    struct cobs_config_t {
        uint8_t delimiter;
        uint8_t depth;
        uint8_t size_of_the_sequence_to_be_replaced;
        uint8_t byte_of_the_sequence_to_be_replaced;
    };

    namespace core {
        enum fsm_state_t {
            SERVICE_BYTE, REGULAR_BYTE, SWAP_BYTE, REPLACEMENT_SEQUENCE
        };
    }
}

#endif //BDSP_STREAMS_COBS_TYPES_H
