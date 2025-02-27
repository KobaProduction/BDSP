#include "BDSP/strategies/abstract/read.h"

using namespace BDSP::strategies;
using namespace BDSP::strategies::abstract;

void AbstractReadStrategy::init(strategy_data_callback_t read_callback,
                                strategy_ready_state_callback_t ready_callback,
                                void *ctx) {
    _data_callback = read_callback;
    if (not _data_callback) {
        _data_callback = _default_read_callback;
    }
    _ready_state_callback = ready_callback;
    if (not _ready_state_callback) {
        _ready_state_callback = _default_ready_state_callback;
    }
    _context = ctx;
    _init();
    _ready_state_callback(_data_callback not_eq _default_read_callback, _context);
}
