#ifndef BDSP_COBSREADER_H
#define BDSP_COBSREADER_H

#include <./COBS.h>

enum cobs_read_state {PACKED_ID, SIZE_A, SIZE_B, DATA, DELIMITER};

class COBSReader {
public:
    explicit COBSReader(cobs_config_t config);
    ~COBSReader();
    void reset();
private:
    cobs_read_state state = PACKED_ID;
};

#endif //BDSP_COBSREADER_H
