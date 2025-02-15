#ifndef BDSP_STREAMS_ABSTRACT_READER_H
#define BDSP_STREAMS_ABSTRACT_READER_H

#include <stdint.h>

#include "BDSP/streams/abstract/mixins.h"
#include "BDSP/streams/types.h"

namespace BDSP::streams::core {
class AbstractStreamReadStrategy: public streams::core::IStreamReadingStrategy {
private:
    static void _default_read_callback(uint8_t byte, read_status_t status, void *ctx) { };
    static void _default_ready_state_callback(bool state, void *ctx) { };

protected:
    stream_data_handler_t _read_callback = _default_read_callback;
    core::strategy_ready_state_callback_t _ready_state_callback = _default_ready_state_callback;
    void *_context = nullptr;

    virtual void _init() { };

public:
    void init(stream_data_handler_t read_callback,
              core::strategy_ready_state_callback_t ready_callback,
              void *ctx) final;
};
} // namespace BDSP::streams::core

#endif // BDSP_STREAMS_ABSTRACT_READER_H
