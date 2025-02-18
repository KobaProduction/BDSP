#ifndef BDSP_STREAMS_STRATEGIES_ABSTRACT_WRITE_STRATEGY_H
#define BDSP_STREAMS_STRATEGIES_ABSTRACT_WRITE_STRATEGY_H

#include <stdint.h>

#include "BDSP/streams/strategies/types.h"

namespace BDSP::streams::strategies::abstract {

class AbstractWriteStrategy: public BDSP::streams::strategies::IStreamWriteStrategy {
private:
    static void _default_write_handler(uint8_t, void *) { };
    static void _default_ready_state_callback(bool, void *) { };

protected:
    void *_context = nullptr;
    strategy_ready_state_callback_t _ready_state_callback = _default_ready_state_callback;
    strategy_write_handler_t _write_handler = _default_write_handler;

    virtual void _init() { };
public:
    void init(strategy_write_handler_t write_handler,
              strategy_ready_state_callback_t ready_state_callback,
              void *ctx) override;
    void finish() override;
    void write(uint8_t byte) override = 0;
};

} // namespace BDSP::streams::strategies::abstract

#endif // BDSP_STREAMS_STRATEGIES_ABSTRACT_WRITE_STRATEGY_H
