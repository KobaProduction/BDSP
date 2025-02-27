#include "BDSP/strategies/abstract/write.h"

using namespace BDSP::strategies;
using namespace BDSP::strategies::abstract;

void AbstractWriteStrategy::init(strategy_write_handler_t write_handler,
                                 strategy_ready_state_callback_t ready_state_callback,
                                 void *ctx) {
    _ready_state_callback = ready_state_callback;
    if (not _ready_state_callback) {
        _ready_state_callback = _default_ready_state_callback;
    }
    _write_handler = write_handler;
    if (not _write_handler) {
        _write_handler = _default_write_handler;
    }
    _context = ctx;
    _ready_state_callback(_write_handler not_eq _default_write_handler, _context);
    _init();
}

void AbstractWriteStrategy::finish() {
    send_delimiter();
}
