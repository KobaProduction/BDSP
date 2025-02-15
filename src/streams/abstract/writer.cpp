#include "BDSP/streams/abstract/writer.h"

using namespace BDSP::streams::core;

void AbstractStreamWriteStrategy::init(stream_writer_t write_handler,
                                       core::strategy_ready_state_callback_t ready_state_callback,
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

void AbstractStreamWriteStrategy::finish() {
    send_delimiter();
}
