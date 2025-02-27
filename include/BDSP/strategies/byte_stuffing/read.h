#ifndef BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_READ_STRATEGY_H
#define BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_READ_STRATEGY_H

#include "stdint.h"

#include "BDSP/strategies/abstract/read.h"
#include "BDSP/strategies/byte_stuffing/mixins.h"
#include "BDSP/strategies/byte_stuffing/types.h"

namespace BDSP::strategies::byte_stuffing {

namespace core {
class ByteStuffingReadStrategyCore: public abstract::AbstractReadStrategy, public virtual ByteStuffingConfigsMixin {
protected:
    byte_stuffing_config_t _cfg;
    bool _is_escape_state = false;

    void _init() override;
    strategies::strategy_read_status_t _send_status_and_reset(uint8_t byte, strategies::strategy_read_status_t status);

public:
    byte_stuffing_config_t get_config();
    strategies::strategy_read_status_t read(uint8_t byte) override;
    void reset_read_state() override;
    set_byte_stuffing_config_status set_config(byte_stuffing_config_t config);
};

class PPPReadStrategyCore: public ByteStuffingReadStrategyCore, public virtual PPPConfigsMixin { };

} // namespace core

class ByteStuffingReadStrategy final: public core::ByteStuffingReadStrategyCore { };
class PPPReadStrategy final: public core::PPPReadStrategyCore { };

} // namespace BDSP::strategies::byte_stuffing

#endif // BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_READ_STRATEGY_H
