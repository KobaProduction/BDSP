#ifndef BDSP_STREAMS_ABSTRACT_WRITER_H
#define BDSP_STREAMS_ABSTRACT_WRITER_H

#include <stdint.h>

#include "BDSP/streams/abstract/mixins.h"
#include "BDSP/streams/types.h"

namespace BDSP::streams::core {

class AbstractStreamWriteStrategy: public streams::core::IStreamWritingStrategy {
private:
    static void _default_write_handler(uint8_t byte, void *ctx) { };
    static void _default_ready_state_callback(bool state, void *ctx) { };

protected:
    stream_writer_t _write_handler = _default_write_handler;
    core::strategy_ready_state_callback_t _ready_state_callback = _default_ready_state_callback;
    void *_context = nullptr;

    virtual void _init() { };

public:
    void init(stream_writer_t write_handler,
              core::strategy_ready_state_callback_t ready_state_callback,
              void *ctx) override;
    void finish() override;
    void write(uint8_t byte) override = 0;
};

} // namespace BDSP::streams::core

#endif // BDSP_STREAMS_ABSTRACT_WRITER_H
