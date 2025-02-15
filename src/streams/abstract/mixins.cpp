#include "BDSP/streams/abstract/mixins.h"

using namespace BDSP::streams::core;

void StreamReadyMixin::_update_ready_state() {
    _state = _states.handler and _states.strategy ? READY : NOT_READY;
}

void StreamReadyMixin::_set_handler_state(bool is_set) {
    _states.handler = is_set;
    _update_ready_state();
}

void StreamReadyMixin::_set_strategy_state(bool is_ready) {
    _states.strategy = is_ready;
    _update_ready_state();
}
