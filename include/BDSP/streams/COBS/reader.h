#ifndef BDSP_STREAMS_COBS_READER_H
#define BDSP_STREAMS_COBS_READER_H

#include "BDSP/streams/COBS/types.h"
#include "BDSP/streams/abstract/reader.h"

namespace BDSP::streams::COBS {

namespace core {
class COBSReaderCore: public ABS::AbstractReader {
public:
    typedef enum { SERVICE_BYTE, REGULAR_BYTE, SWAP_BYTE} fsm_state_t;
protected:
    cobs_config_t _cfg{};
    set_cobs_config_status (*_config_checker)(cobs_config_t config){};
    fsm_state_t _fsm_state = SERVICE_BYTE;
    uint8_t _service_byte_offset = _cfg.depth;
    uint8_t _swap_byte_offset{};

    virtual void _exec_delimiter(uint8_t byte);
    uint8_t _get_converted_swap_byte_offset(uint8_t raw_offset);
    read_status_t _process_byte(uint8_t byte) override;
    void _reset() override;
    virtual read_status_t _set_swap_byte_offset(uint8_t offset);

public:
    cobs_config_t get_config();
    set_cobs_config_status set_config(cobs_config_t config);
    explicit COBSReaderCore();
};

class COBSSRReaderCore: public COBSReaderCore {
protected:
    uint8_t _sequence_replace_length_threshold{};
    bool _is_sequence_replacement_state = false;

    read_status_t _process_byte(uint8_t byte) override;
    read_status_t _set_swap_byte_offset(uint8_t offset) override;

public:
    explicit COBSSRReaderCore();
};

class COBSZPEReaderCore: public COBSSRReaderCore {
public:
    explicit COBSZPEReaderCore();
};
}
class COBSReader final: public core::COBSReaderCore { };

class COBSSRReader final: public core::COBSSRReaderCore { };

class COBSZPEReader final: public core::COBSZPEReaderCore { };

} // namespace BDSP::streams::COBS

#endif // BDSP_STREAMS_COBS_READER_H
