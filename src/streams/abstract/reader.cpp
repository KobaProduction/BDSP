#include "BDSP/streams/abstract/reader.h"

using namespace BDSP::streams;
using namespace BDSP::streams::core;
using namespace BDSP::streams::core;

void AbstractStreamReadStrategy::init(stream_data_handler_t read_callback,
                                      strategy_ready_state_callback_t ready_callback,
                                            void *ctx) {
    _read_callback = read_callback;
    if (not _read_callback) {
        _read_callback = _default_read_callback;
    }
    _ready_state_callback = ready_callback;
    if (not _ready_state_callback) {
        _ready_state_callback = _default_ready_state_callback;
    }
    _context = ctx;
    _ready_state_callback(_read_callback not_eq _default_read_callback, _context);
    _init();
}

