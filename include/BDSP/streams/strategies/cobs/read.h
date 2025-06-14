#ifndef BDSP_STREAMS_STRATEGIES_COBS_READ_STRATEGIES_H
#define BDSP_STREAMS_STRATEGIES_COBS_READ_STRATEGIES_H

#include "BDSP/streams/strategies/COBS/mixins.h"
#include "BDSP/streams/strategies/COBS/types.h"
#include "BDSP/streams/strategies/abstract/read.h"

#include "BDSP/streams/strategies/types.h"

namespace BDSP::streams::strategies::cobs {

namespace core {
class COBSReadStrategyCore: public strategies::abstract::AbstractReadStrategy, public virtual COBSConfigsMixin {
public:
    typedef enum { SERVICE_BYTE, REGULAR_BYTE, SWAP_BYTE } fsm_state_t;

protected:
    cobs_config_t _cfg{};
    fsm_state_t _fsm_state = SERVICE_BYTE;
    uint8_t _service_byte_offset = _cfg.depth;
    uint8_t _swap_byte_offset{};

    virtual void _exec_delimiter(uint8_t byte);
    uint8_t _get_converted_swap_byte_offset(uint8_t raw_offset);
    virtual bool _get_read_process_state();
    void _init() override;
    virtual strategy_read_status_t _set_swap_byte_offset(uint8_t offset);

public:
    cobs_config_t get_config();
    strategy_read_status_t read(uint8_t byte) override;
    void reset_read_state() override;
    set_cobs_config_status set_config(cobs_config_t config);
};

class COBSSRReadStrategyCore: public COBSReadStrategyCore, public virtual COBSSRConfigsMixin {
protected:
    uint8_t _sequence_replace_length_threshold{};
    bool _is_sequence_replacement_state = false;

    void _exec_delimiter(uint8_t byte) override;
    bool _get_read_process_state() override;
    strategy_read_status_t _set_swap_byte_offset(uint8_t offset) override;

public:
    strategy_read_status_t read(uint8_t byte) override;
    void reset_read_state() override;
};

class COBSZPEReadStrategyCore: public COBSSRReadStrategyCore, public virtual COBSZPEConfigsMixin {
};

} // namespace core

class COBSReadStrategy final: public core::COBSReadStrategyCore { };
class COBSSRReadStrategy final: public core::COBSSRReadStrategyCore { };
class COBSZPEReadStrategy final: public core::COBSZPEReadStrategyCore { };

} // namespace BDSP::streams::strategies::cobs

#endif // BDSP_STREAMS_STRATEGIES_COBS_READ_STRATEGIES_H
