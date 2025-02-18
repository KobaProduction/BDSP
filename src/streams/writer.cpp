#include "BDSP/streams/writer.h"

using namespace BDSP::streams;
using namespace BDSP::streams::core;

void StreamWriterCore::set_strategy(strategies::IStreamWriteStrategy &strategy) noexcept {
    _strategy = &strategy;
    _strategy->init(
        [](uint8_t byte, void *ctx) {
            auto *self = reinterpret_cast<StreamWriterCore *>(ctx);
            self->_handler(byte, self->_handler_context);
        },
        [](bool new_ready_state, void *ctx) {
            reinterpret_cast<StreamWriterCore *>(ctx)->_set_strategy_state(new_ready_state);
        },
        this);
}

bool StreamWriterCore::get_ready_status() {
    return _state == READY;
}

stream_write_status_t StreamWriterCore::finish() {
    if (_state not_eq READY) {
        return STREAM_WRITER_NOT_READY_ERROR;
    }
    _strategy->finish();
    return STREAM_WRITE_DELIMITER;
}

void StreamWriterCore::set_stream_writer(stream_write_handler_t handler, void *context_ptr) {
    _handler = handler;
    _handler_context = context_ptr;
    _set_handler_state(_handler not_eq nullptr);
}

stream_write_status_t StreamWriterCore::write(uint8_t byte) {
    if (_state not_eq READY) {
        return STREAM_WRITER_NOT_READY_ERROR;
    }
    _strategy->write(byte);
    return STREAM_WRITE_OK;
}

stream_write_status_t StreamWriterCore::write(uint8_t *buffer_ptr, size_t size) {
    stream_write_status_t status = STREAM_WRITE_OK;
    for (size_t i = 0; i < size; ++i) {
        status = write(buffer_ptr[i]);
        if (status not_eq STREAM_WRITE_OK) {
            break;
        }
    }
    return status;
}
