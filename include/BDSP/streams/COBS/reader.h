#ifndef BDSP_STREAMS_COBS_DECODER_H
#define BDSP_STREAMS_COBS_DECODER_H

#include "BDSP/streams/COBS/types.h"
#include "BDSP/streams/abstract/reader.h"

namespace BDSP::streams::COBS {

class COBSReader: public ABS::AbstractReader {
protected:
    cobs_config_t _cfg{};
    core::fsm_state_t _fsm_state{};
    uint8_t _service_byte_offset{};
    uint8_t _swap_byte_offset{};

    bool next_swap_byte_is_place_of_the_replaced_sequence = false;

    read_status_t _process_byte(uint8_t byte) override;

    void _reset() override;

    virtual read_status_t _set_swap_byte_offset(uint8_t offset);

public:
    explicit COBSReader();
    virtual set_config_status set_config(cobs_config_t config);
};

class COBSZPEReader final: public COBSReader {
    read_status_t _set_swap_byte_offset(uint8_t offset) override;
public:
    explicit COBSZPEReader();
    set_config_status set_config(cobs_config_t config) override;
};
} // namespace BDSP::streams::COBS

#endif // BDSP_STREAMS_COBS_DECODER_H
