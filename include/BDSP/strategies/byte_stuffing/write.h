#ifndef BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_WRITE_STRATEGY_H
#define BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_WRITE_STRATEGY_H

#include <stdint.h>

#include "BDSP/strategies/abstract/write.h"
#include "BDSP/strategies/byte_stuffing/mixins.h"
#include "BDSP/strategies/byte_stuffing/types.h"

namespace BDSP::strategies::byte_stuffing {

namespace core {
class ByteStuffingWriteStrategyCore: public abstract::AbstractWriteStrategy, public virtual ByteStuffingConfigsMixin {
protected:
    byte_stuffing_config_t _cfg;
    void _init() override;

public:
    byte_stuffing_config_t get_config();
    void send_delimiter() override;
    set_byte_stuffing_config_status set_config(byte_stuffing_config_t config);
    void write(uint8_t byte) override;
};

class PPPWriteStrategyCore: public ByteStuffingWriteStrategyCore, public virtual PPPConfigsMixin { };

} // namespace core

class ByteStuffingWriteStrategy final: public core::ByteStuffingWriteStrategyCore { };
class PPPStreamWriteStrategy final: public core::PPPWriteStrategyCore { };

} // namespace BDSP::strategies::byte_stuffing

#endif // BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_WRITE_STRATEGY_H
