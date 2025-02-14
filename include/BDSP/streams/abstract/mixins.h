#ifndef BDSP_STREAMS_ABSTRACT_MIXINS_H
#define BDSP_STREAMS_ABSTRACT_MIXINS_H

namespace BDSP::streams::core {

class StreamReadyMixin {
public:
    enum StreamReadyState { NOT_READY, READY };

private:
    struct {
        bool handler : 1;
        bool strategy : 1;
    } states = {false, true};
    void _update_ready_state();

protected:
    StreamReadyState _state = NOT_READY;
    void _set_handler_state(bool is_set);
    void _set_strategy_state(bool is_ready);
};
} // namespace BDSP::streams::core

#endif // BDSP_STREAMS_ABSTRACT_MIXINS_H
