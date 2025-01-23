#ifndef BDSP_STREAMS_COBS_TYPES_H
#define BDSP_STREAMS_COBS_TYPES_H

#ifndef MIN_BDSP_COBS_DEPTH
#    define MIN_BDSP_COBS_DEPTH 16
#endif

#include <stdint.h>

namespace BDSP::streams::COBS {

struct cobs_config_t {
    uint8_t delimiter = '\0';
    uint8_t depth = 255;
    uint8_t size_of_the_sequence_to_be_replaced = 0;
    uint8_t byte_of_the_sequence_to_be_replaced = '\0';
};

enum set_config_status {
    /**
     * Returned in case of successful configuration installation.
     */
    SET_OK = 0,
    /**
     * This error occurs when you change the configuration with an uncompleted data  transmission.
     * Will not be guaranteed to work because it is code not checked whether delimiter was sent earlier.
     * In order to avoid errors, it is better to change the configuration only after creating an object and before
     * sending data.
     */
    ERROR_PROCESS_NOT_FINISHED,
    /**
     * This error occurs when memory to encoding buffer has not been acquired (got nullptr).
     */
    ERROR_MEMORY_ALLOCATION,
    /**
     * This error occurs when the search depth of a delimiter byte is less than
     * MIN_BDSP_COBS_DEPTH value (default value 16).
     */
    ERROR_COBS_DEPTH,
    /**
     * This warning occurs if the length of the sequence being replaced is not 0.
     * The standard COBS implementation does not use this option.
     * To replace the sequence, you need to use COBS/SR or COBS/ZPE algorithms.
     */
    ERROR_DEFAULT_COBS_SIZE_SR,
    /**
     * This error occurs when the length of the sequence being replaced is less than 2 in a COBS/SR or COBS/ZPE configuration.
     * Logically, you need 2 or more bytes to replace the sequence.
     */
    ERROR_SIZE_SR,
    /**
     * This error occurs when a sequence replacement algorithm (only COBS/SR) is used and the length of the separator byte lookup is
     * greater than 127. One of the eight bits is required to mark the location of the sequence to be replaced, so the
     * total length of the search should not be more than half of the maximum value of 255.
     */
    ERROR_DEPTH_SR,
    /**
     * This warning occurs when the search length of the delimiter byte in the configuration is not 224 (0xE0).
     * The value cannot be changed at this time, but if testing shows the effectiveness of other values, this part can
     * be modified.
     */
    ERROR_DEPTH_ZPE,
};

namespace core {
enum fsm_state_t { SERVICE_BYTE, REGULAR_BYTE, SWAP_BYTE, REPLACEMENT_SEQUENCE };
}

} // namespace BDSP::streams::COBS

#endif // BDSP_STREAMS_COBS_TYPES_H
