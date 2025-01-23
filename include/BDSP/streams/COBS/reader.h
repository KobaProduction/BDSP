#ifndef BDSP_STREAMS_COBS_DECODER_H
#define BDSP_STREAMS_COBS_DECODER_H

#include "BDSP/streams/COBS/checkers.h"
#include "BDSP/streams/COBS/types.h"
#include "BDSP/streams/abstract/reader.h"

namespace BDSP::streams::COBS {

namespace core {
class COBSReaderCore: public ABS::AbstractReader {
protected:
    cobs_config_t _cfg{};
    core::fsm_state_t _fsm_state = SWAP_BYTE;
    uint8_t _service_byte_offset = 0;
    uint8_t _swap_byte_offset{};

    uint8_t _get_converted_swap_byte_offset(uint8_t raw_offset);

    read_status_t _process_byte(uint8_t byte) override;

    void _reset() final;

    set_config_status _set_config(cobs_config_t config);

    virtual read_status_t _set_swap_byte_offset(uint8_t offset);

public:
    cobs_config_t get_config();
    virtual set_config_status set_config(cobs_config_t config);
};

class COBSSRReaderCore: public COBSReaderCore {
protected:
    uint8_t _sequence_replace_length_threshold = 127;

    bool _next_swap_byte_is_place_of_the_replaced_sequence = false;
    read_status_t _process_byte(uint8_t byte) override;
    read_status_t _set_swap_byte_offset(uint8_t offset) override;
public:
    set_config_status set_config(cobs_config_t config) override;
};

class COBSZPEReaderCore: public COBSSRReaderCore {
public:
    explicit COBSZPEReaderCore();

    set_config_status set_config(cobs_config_t config) override;
};
} // namespace core


class COBSReader final: public core::COBSReaderCore {
public:
    explicit COBSReader();
};

class COBSSRReader final: public core::COBSSRReaderCore {
public:
    explicit COBSSRReader();
};

class COBSZPEReader final: public core::COBSZPEReaderCore {
public:
    explicit COBSZPEReader();
};

} // namespace BDSP::streams::COBS

#endif // BDSP_STREAMS_COBS_DECODER_H
