#ifndef BDSP_STREAMS_ABS_STREAM_H
#define BDSP_STREAMS_ABS_STREAM_H

namespace BDSP::streams::ABS {

enum StreamReadyState { NOT_READY, READY };

struct StreamReadyStates {
    bool ready : 1;
    bool handler : 1;
};

class AbstractStreamReadyMixin {
    StreamReadyStates states = {true, false};

protected:
    StreamReadyState _state = NOT_READY;

public:
    void _set_handler_state(bool is_set);
    void _set_ready_state(bool is_ready);
};
} // namespace BDSP::streams::ABS

#endif // BDSP_STREAMS_ABS_STREAM_H
