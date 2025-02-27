#ifndef BDSP_STREAMS_STRATEGIES_ABSTRACT_WRITE_STRATEGY_H
#define BDSP_STREAMS_STRATEGIES_ABSTRACT_WRITE_STRATEGY_H

#include <stdint.h>

#include "BDSP/strategies/types.h"

namespace BDSP::strategies::abstract {

class AbstractWriteStrategy: public strategies::IStreamWriteStrategy {
private:
    static void _default_write_handler(uint8_t, void *) { };
    static void _default_ready_state_callback(bool, void *) { };

protected:
    void *_context = nullptr;
    strategy_ready_state_callback_t _ready_state_callback = _default_ready_state_callback;
    strategy_write_handler_t _write_handler = _default_write_handler;

    virtual void _init() = 0;

public:
    void init(strategy_write_handler_t write_handler,
              strategy_ready_state_callback_t ready_state_callback,
              void *ctx) override;
    void finish() override;
};

} // namespace BDSP::strategies::abstract

#endif // BDSP_STREAMS_STRATEGIES_ABSTRACT_WRITE_STRATEGY_H
