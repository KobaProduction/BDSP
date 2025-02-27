#ifndef BDSP_STREAMS_STRATEGIES_ABSTRACT_READ_STRATEGY_H
#define BDSP_STREAMS_STRATEGIES_ABSTRACT_READ_STRATEGY_H

#include <stdint.h>

#include "BDSP/strategies/types.h"

namespace BDSP::strategies::abstract {

class AbstractReadStrategy: public strategies::IStreamReadStrategy {
private:
    static void _default_read_callback(uint8_t, strategies::strategy_read_status_t, void *) { };
    static void _default_ready_state_callback(bool, void *) { };

protected:
    void *_context = nullptr;
    strategy_data_callback_t _data_callback = _default_read_callback;
    strategy_ready_state_callback_t _ready_state_callback = _default_ready_state_callback;

    virtual void _init() = 0;

public:
    void init(strategies::strategy_data_callback_t read_callback,
              strategies::strategy_ready_state_callback_t ready_callback,
              void *ctx) final;
};
} // namespace BDSP::strategies::abstract

#endif // BDSP_STREAMS_STRATEGIES_ABSTRACT_READ_STRATEGY_H
