#ifndef BDSP_STREAMS_STRATEGIES_COBS_WRITE_STRATEGIES_H
#define BDSP_STREAMS_STRATEGIES_COBS_WRITE_STRATEGIES_H

#include <stdlib.h>

#include "BDSP/strategies/abstract/write.h"
#include "BDSP/strategies/cobs/mixins.h"
#include "BDSP/strategies/cobs/types.h"

namespace BDSP::strategies::cobs {
namespace core {

class COBSWriteStrategyCore: public abstract::AbstractWriteStrategy, public virtual COBSConfigsMixin {
protected:
    uint8_t *_buffer_ptr{};
    uint8_t _buffer_position{};
    cobs_config_t _cfg{};
    void *(*_malloc)(size_t) = malloc;
    void (*_free)(void *) = free;

    void _encode(uint8_t byte);
    virtual bool _get_active_write_state_status();
    void _init() override;
    void _write_buffer_with_offset_to_handler(uint8_t offset);

public:
    ~COBSWriteStrategyCore() override;
    cobs_config_t get_config();
    void finish() override;
    void send_delimiter() override;
    virtual set_cobs_config_status set_config(cobs_config_t config);
    void write(uint8_t byte) override;
};

class COBSGSEWriteStrategyCore: public COBSWriteStrategyCore, public virtual COBSGSEConfigsMixin {
protected:
    uint8_t _counter_of_the_sequence_to_be_replaced{};
    uint8_t _limit_position_of_the_sequence_to_be_replaced{};
    bool _get_active_write_state_status() override;
    void _reset_counter_of_the_sequence_to_be_replaced();

public:
    void finish() override;
    set_cobs_config_status set_config(cobs_config_t config) override;
    void write(uint8_t byte) override;
};

class COBSZPEWriteStrategyCore: public COBSGSEWriteStrategyCore, public virtual COBSZPEConfigsMixin { };

} // namespace core

class COBSWriteStrategy final: public core::COBSWriteStrategyCore { };
class COBSGSEWriteStrategy final: public core::COBSGSEWriteStrategyCore { };
class COBSZPEWriteStrategy final: public core::COBSZPEWriteStrategyCore { };
} // namespace BDSP::strategies::cobs

#endif // BDSP_STREAMS_STRATEGIES_COBS_WRITE_STRATEGIES_H
