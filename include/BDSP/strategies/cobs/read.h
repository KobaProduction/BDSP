#ifndef BDSP_STREAMS_STRATEGIES_COBS_READ_STRATEGIES_H
#define BDSP_STREAMS_STRATEGIES_COBS_READ_STRATEGIES_H

#include "BDSP/strategies/abstract/read.h"
#include "BDSP/strategies/cobs/mixins.h"
#include "BDSP/strategies/cobs/types.h"
#include "BDSP/strategies/types.h"

namespace BDSP::strategies::cobs {

namespace core {
class COBSReadStrategyCore: public abstract::AbstractReadStrategy, public virtual COBSConfigsMixin {
protected:
    cobs_config_t _cfg{};
    uint8_t _offset_to_service_byte = 0;
    bool _service_byte_type_is_offset = true;

    virtual void _exec_delimiter(uint8_t byte);
    virtual bool _exec_new_offset_and_get_success_status(uint8_t offset);
    uint8_t _get_converted_swap_byte_offset(uint8_t raw_offset);
    virtual bool _get_read_process_state();
    void _init() override;

public:
    cobs_config_t get_config();
    strategy_read_status_t read(uint8_t byte) override;
    void reset_read_state() override;
    set_cobs_config_status set_config(cobs_config_t config);
};

class COBSSRReadStrategyCore: public COBSReadStrategyCore, public virtual COBSSRConfigsMixin {
protected:
    bool _is_sequence_replacement_state = false;

    void _exec_delimiter(uint8_t byte) override;
    bool _exec_new_offset_and_get_success_status(uint8_t offset) override;
    bool _get_read_process_state() override;

public:
    void reset_read_state() override;
};

class COBSZPEReadStrategyCore: public COBSSRReadStrategyCore, public virtual COBSZPEConfigsMixin { };

} // namespace core

class COBSReadStrategy final: public core::COBSReadStrategyCore { };
class COBSSRReadStrategy final: public core::COBSSRReadStrategyCore { };
class COBSZPEReadStrategy final: public core::COBSZPEReadStrategyCore { };

} // namespace BDSP::strategies::cobs

#endif // BDSP_STREAMS_STRATEGIES_COBS_READ_STRATEGIES_H
