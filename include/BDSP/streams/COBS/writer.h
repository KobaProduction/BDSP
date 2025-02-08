#ifndef BDSP_STREAMS_COBS_WRITER_H
#define BDSP_STREAMS_COBS_WRITER_H

#include "BDSP/streams/COBS/types.h"
#include "BDSP/streams/abstract/writer.h"
#include <stdlib.h>

namespace BDSP::streams::COBS {

namespace core {

class COBSWriterCore: public ABS::AbstractStreamWriter {
protected:
    void *(*_malloc)(size_t) = malloc;
    void (*_free)(void *) = free;
    cobs_config_t _cfg{};
    set_cobs_config_status (*_config_checker)(cobs_config_t config){};
    uint8_t *_buffer_ptr = nullptr;
    uint8_t _buffer_position = 1;

    void _encode(uint8_t byte);
    void _finish() override;
    void _process_byte(uint8_t byte) override;
    void _write_buffer(uint8_t offset_value);

public:
    cobs_config_t get_config();
    virtual set_cobs_config_status set_config(cobs_config_t config);
    COBSWriterCore();
    ~COBSWriterCore();
};

class COBSSRWriterCore: public COBSWriterCore {
protected:
    uint8_t _current_size_of_the_sequence_to_be_replaced = 0;

    void _finish() override;
    void _process_byte(uint8_t byte) override;
    void _reset_elimination_sequence();

public:
    set_cobs_config_status set_config(cobs_config_t config) override;
    explicit COBSSRWriterCore();
};

class COBSZPEWriterCore: public COBSSRWriterCore {
    void _process_byte(uint8_t byte) final;

public:
    explicit COBSZPEWriterCore();
};
} // namespace core

class COBSWriter final: public core::COBSWriterCore {
public:
    explicit COBSWriter() noexcept;
};

class COBSSRWriter final: public core::COBSSRWriterCore {
public:
    explicit COBSSRWriter() noexcept;
};

class COBSZPEWriter final: public core::COBSZPEWriterCore {
public:
    explicit COBSZPEWriter() noexcept;
};

} // namespace BDSP::streams::COBS

#endif // BDSP_STREAMS_COBS_WRITER_H
