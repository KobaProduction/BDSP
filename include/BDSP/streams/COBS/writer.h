#ifndef BDSP_STREAMS_COBS_ENCODER_H
#define BDSP_STREAMS_COBS_ENCODER_H

#include "BDSP/streams/COBS/types.h"
#include "BDSP/streams/abstract/writer.h"

namespace BDSP::streams::COBS {

class COBSWriter final: public ABS::AbstractWriter {
    COBS::cobs_config_t _cfg{};
    uint8_t *_buffer_ptr = nullptr;
    uint8_t _buffer_position = 1;
    uint8_t _current_size_of_the_sequence_to_be_replaced = 0;

    void _encode_default(uint8_t byte);

    void _process_byte(uint8_t byte) override;

    void _finish() override;

    void _reset_elimination_sequence();

    void _write_buffer(bool is_elimination_sequence = false);

public:
    explicit COBSWriter();

    ~COBSWriter();

    set_config_status set_config(COBS::cobs_config_t config);
};

} // namespace BDSP::streams::COBS

#endif // BDSP_STREAMS_COBS_ENCODER_H
