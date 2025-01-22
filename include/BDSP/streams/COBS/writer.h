#ifndef BDSP_STREAMS_COBS_ENCODER_H
#define BDSP_STREAMS_COBS_ENCODER_H

#include "BDSP/streams/COBS/types.h"
#include "BDSP/streams/abstract/writer.h"
#include "BDSP/types.h"

namespace BDSP::streams::COBS {

class COBSWriter: public ABS::AbstractWriter {
protected:
    BDSP::core::bdsp_memory_allocator_t _malloc = malloc;
    BDSP::core::bdsp_memory_cleaner_t _free = free;
    bool (*_set_config)(cobs_config_t &config, set_config_status &status){};
    COBS::cobs_config_t _cfg{};
    uint8_t *_buffer_ptr = nullptr;
    uint8_t _buffer_position = 1;

    void _encode(uint8_t byte);
    void _finish() override;
    void _process_byte(uint8_t byte) override;
    void _write_buffer(uint8_t cobs_offset_value = 0);

public:
    ~COBSWriter();
    explicit COBSWriter();
    COBS::cobs_config_t get_config();
    set_config_status set_config(COBS::cobs_config_t config);
};

class COBSSRWriter: public COBSWriter {
protected:
    uint8_t _current_size_of_the_sequence_to_be_replaced = 0;

    void _finish() override;
    void _process_byte(uint8_t byte) override;
    void _reset_elimination_sequence();

public:
    explicit COBSSRWriter();
};

class COBSZPEWriter final: public COBSSRWriter {
    void _process_byte(uint8_t byte) override;

public:
    explicit COBSZPEWriter();
};

} // namespace BDSP::streams::COBS

#endif // BDSP_STREAMS_COBS_ENCODER_H
