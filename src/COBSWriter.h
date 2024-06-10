#ifndef BDSP_COBSWRITER_H
#define BDSP_COBSWRITER_H

#include <./COBS.h>

enum cobs_writer_status_t {
    COBS_OK,
    COBS_BUFFER_MISSING, // Buffer missing. There may be insufficient RAM.
    COBS_EMPTY_DATA,
};

class COBSWriter {
public:
    explicit COBSWriter(cobs_config_t config, void (*writer_ptr)(uint8_t *data_ptr, size_t size));
    ~COBSWriter();
    cobs_writer_status_t get_status();
    void reset();
    void send_segment(uint8_t *data_ptr, size_t size);
    cobs_writer_status_t finish_sending(bool is_send_with_delimiter = true);
private:
    cobs_config_t config = {DEFAULT_COBS_DELIMITER, DEFAULT_COBS_DEPTH};
    void (*writer_h) (uint8_t*, size_t) = nullptr;
    uint8_t *buffer_ptr = nullptr;
    uint8_t code = 1;
    uint8_t *dst = nullptr;
    uint8_t *code_ptr = nullptr;
};

#endif //BDSP_COBSWRITER_H
