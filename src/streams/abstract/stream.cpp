#include "BDSP/streams/abstract/stream.h"

using namespace BDSP::streams::ABS;

void AbstractStreamReadyMixin::_set_handler_state(bool is_set) {
    states.handler = is_set;
    _state = states.handler and states.ready ? READY : NOT_READY;
}

void AbstractStreamReadyMixin::_set_ready_state(bool is_ready) {
    states.ready = is_ready;
    _state = states.handler and states.ready ? READY : NOT_READY;
}
