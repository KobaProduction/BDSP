#ifndef BDSP_COBS_H
#define BDSP_COBS_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define DEFAULT_COBS_DEPTH 255
#define DEFAULT_COBS_DELIMITER '\0'

struct cobs_config_t {
    uint8_t delimiter; // Maximum value is 254.
    uint8_t depth;  // The depth and separator parameters must not be equal.
};

#ifndef BDSP_COBSWRITER_H
#include <./COBSWriter.h>
#endif

#ifndef BDSP_COBSREADER_H
#include <./COBSReader.h>
#endif

#endif //BDSP_COBS_H
