#ifndef BDSP_READER_H
#define BDSP_READER_H

#include "./config.h"

enum cobs_read_state {PACKED_ID, SIZE_A, SIZE_B, DATA, DELIMITER};

class COBSReader {
public:
    explicit COBSReader(cobs_config_t config);
    ~COBSReader();
    void reset();
private:
    cobs_read_state state = PACKED_ID;
};

#endif //BDSP_READER_H
