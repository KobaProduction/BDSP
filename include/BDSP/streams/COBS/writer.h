#ifndef BDSP_STREAMS_COBS_ENCODER_H
#define BDSP_STREAMS_COBS_ENCODER_H

#include "BDSP/streams/COBS/types.h"
#include "BDSP/streams/abstract/writer.h"

namespace BDSP::streams::COBS {

class COBSWriter: public ABS::AbstractWriter {
protected:
    COBS::cobs_config_t _cfg{};
    uint8_t *_buffer_ptr = nullptr;
    uint8_t _buffer_position = 1;
    uint8_t _current_size_of_the_sequence_to_be_replaced = 0;

    void _encode_default(uint8_t byte);

    void _process_byte(uint8_t byte) override;

    void _finish() override;

    void _reset_elimination_sequence();

    void _write_buffer(uint8_t cobs_offset_value = 0);

    bool _create_buffer_and_set_config();

public:
    explicit COBSWriter();

    ~COBSWriter();

    virtual set_config_status set_config(COBS::cobs_config_t config);
};

class COBSZPEWriter final: public COBSWriter {
    void _process_byte(uint8_t byte) override;
public:
    explicit COBSZPEWriter();

    set_config_status set_config(COBS::cobs_config_t config) override;
};


} // namespace BDSP::streams::COBS

#endif // BDSP_STREAMS_COBS_ENCODER_H
