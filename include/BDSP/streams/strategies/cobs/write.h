#ifndef BDSP_STREAMS_STRATEGIES_COBS_WRITE_STRATEGIES_H
#define BDSP_STREAMS_STRATEGIES_COBS_WRITE_STRATEGIES_H

#include <stdlib.h>

#include "BDSP/streams/strategies/COBS/mixins.h"
#include "BDSP/streams/strategies/COBS/types.h"
#include "BDSP/streams/strategies/abstract/write.h"

namespace BDSP::streams::strategies::cobs {
namespace core {

class COBSWriteStrategyCore: public abstract::AbstractWriteStrategy, public virtual COBSConfigsMixin {
protected:
    uint8_t *_buffer_ptr = nullptr;
    uint8_t _buffer_position = 1;
    cobs_config_t _cfg{};
    void *(*_malloc)(size_t) = malloc;
    void (*_free)(void *) = free;

    void _encode(uint8_t byte);
    virtual bool _get_read_process_state();
    void _init() override;
    void _write_buffer_to_stream(uint8_t offset_value);

public:
    ~COBSWriteStrategyCore() override;
    cobs_config_t get_config();
    void finish() override;
    void send_delimiter() override;
    set_cobs_config_status set_config(cobs_config_t config);
    void write(uint8_t byte) override;
};

class COBSSRWriteStrategyCore: public COBSWriteStrategyCore, public virtual COBSSRConfigsMixin {
protected:
    uint8_t _current_size_of_the_sequence_to_be_replaced = 0;
//    void _init() override;
    bool _get_read_process_state() override;
    virtual void _reset_elimination_sequence();

public:
    void finish() override;
    void write(uint8_t byte) override;
};

class COBSZPEWriteStrategyCore: public COBSSRWriteStrategyCore, public virtual COBSZPEConfigsMixin {
//protected:
//    void _init() override;

public:
    void write(uint8_t byte) override;
};

} // namespace core

class COBSWriteStrategy final: public core::COBSWriteStrategyCore { };
class COBSSRWriteStrategy final: public core::COBSSRWriteStrategyCore { };
class COBSZPEWriteStrategy final: public core::COBSZPEWriteStrategyCore { };
} // namespace BDSP::streams::strategies::cobs

#endif // BDSP_STREAMS_STRATEGIES_COBS_WRITE_STRATEGIES_H
