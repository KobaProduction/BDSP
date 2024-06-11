#ifndef BDSP_ENCODERS_COBS_CONFIG_H
#define BDSP_ENCODERS_COBS_CONFIG_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define DEFAULT_COBS_DEPTH 255
#define DEFAULT_COBS_DELIMITER '\0'

struct cobs_config_t {
    uint8_t delimiter;
    uint8_t depth;
};

#endif //BDSP_ENCODERS_COBS_CONFIG_H
