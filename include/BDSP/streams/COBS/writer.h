#ifndef BDSP_STREAMS_COBS_ENCODER_H
#define BDSP_STREAMS_COBS_ENCODER_H

#include "BDSP/streams/COBS/types.h"
#include "BDSP/streams/abstract/writer.h"
#include "BDSP/types.h"

namespace BDSP::streams::COBS {

namespace core {

class COBSWriterCore: public ABS::AbstractWriter {
protected:
    BDSP::core::bdsp_memory_allocator_t _malloc = malloc;
    BDSP::core::bdsp_memory_cleaner_t _free = free;
    cobs_config_t _cfg{};
    uint8_t *_buffer_ptr = nullptr;
    uint8_t _buffer_position = 1;

    void _encode(uint8_t byte);
    void _finish() override;
    void _process_byte(uint8_t byte) override;
    set_config_status _set_config(cobs_config_t config);
    void _write_buffer(uint8_t offset_value);

public:
    ~COBSWriterCore();
    cobs_config_t get_config();
    virtual set_config_status set_config(cobs_config_t config);
};

class COBSSRWriterCore: public COBSWriterCore {
protected:
    uint8_t _current_size_of_the_sequence_to_be_replaced = 0;

    void _finish() override;
    void _process_byte(uint8_t byte) override;
    void _reset_elimination_sequence();

public:
    set_config_status set_config(cobs_config_t config) override;
};

class COBSZPEWriterCore: public COBSSRWriterCore {
    void _process_byte(uint8_t byte) final;

public:
    set_config_status set_config(cobs_config_t config) override;
};
} // namespace core

class COBSWriter final: public core::COBSWriterCore {
public:
    explicit COBSWriter();
};

class COBSSRWriter final: public core::COBSSRWriterCore {
public:
    explicit COBSSRWriter();
};

class COBSZPEWriter final: public core::COBSZPEWriterCore {
public:
    explicit COBSZPEWriter();
};

} // namespace BDSP::streams::COBS

#endif // BDSP_STREAMS_COBS_ENCODER_H
