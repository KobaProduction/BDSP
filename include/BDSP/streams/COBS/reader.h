#ifndef BDSP_STREAMS_COBS_DECODER_H
#define BDSP_STREAMS_COBS_DECODER_H

#include "BDSP/streams/COBS/types.h"
#include "BDSP/streams/abstract/reader.h"

namespace BDSP::streams::COBS {

class COBSReader: public ABS::AbstractReader {
protected:
    cobs_config_t _cfg{};
    set_config_status (*_config_checker)(cobs_config_t config){};
    core::fsm_state_t _fsm_state = core::SERVICE_BYTE;
    uint8_t _service_byte_offset = _cfg.depth;
    uint8_t _swap_byte_offset{};

    uint8_t _get_converted_swap_byte_offset(uint8_t raw_offset);
    read_status_t _process_byte(uint8_t byte) override;
    void _reset() final;
    virtual read_status_t _set_swap_byte_offset(uint8_t offset);

public:
    cobs_config_t get_config();
    set_config_status set_config(cobs_config_t config);
    explicit COBSReader();
};

class COBSSRReader: public COBSReader {
protected:
    uint8_t _sequence_replace_length_threshold{};

    bool _next_swap_byte_is_place_of_the_replaced_sequence = false;
    read_status_t _process_byte(uint8_t byte) override;
    read_status_t _set_swap_byte_offset(uint8_t offset) override;

public:
    explicit COBSSRReader();
};

class COBSZPEReader: public COBSSRReader {
public:
    explicit COBSZPEReader();
};

} // namespace BDSP::streams::COBS

#endif // BDSP_STREAMS_COBS_DECODER_H
