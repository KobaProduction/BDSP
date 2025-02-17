#include "BDSP/streams/writer.h"

using namespace BDSP::streams;
using namespace BDSP::streams::core;

void StreamWriterCore::set_strategy(core::IStreamWritingStrategy &strategy) noexcept {
    _strategy = &strategy;
    _strategy->init(
        [](uint8_t byte, void *ctx) {
            auto *self = reinterpret_cast<StreamWriterCore *>(ctx);
            self->_write_handler(byte, self->_write_handler_context);
        },
        [](bool new_ready_state, void *ctx) {
            reinterpret_cast<StreamWriterCore *>(ctx)->_set_strategy_state(new_ready_state);
        },
        this
    );
}

bool StreamWriterCore::get_ready_status() {
    return _state == READY;
}

write_status_t StreamWriterCore::finish() {
    if (_state not_eq READY) {
        return STREAM_WRITER_NOT_READY_ERROR;
    }
    _strategy->finish();
    return STREAM_WRITE_END;
}

void StreamWriterCore::set_stream_writer(stream_writer_t writer, void *context_ptr) {
    _write_handler = writer;
    _write_handler_context = context_ptr;
    _set_handler_state(_write_handler not_eq nullptr);
}

write_status_t StreamWriterCore::write(uint8_t byte) {
    if (_state not_eq READY) {
        return STREAM_WRITER_NOT_READY_ERROR;
    }
    _strategy->write(byte);
    return STREAM_WRITE_OK;
}

write_status_t StreamWriterCore::write(uint8_t *buffer_ptr, size_t size) {
    write_status_t status = STREAM_WRITE_OK;
    for (size_t i = 0; i < size; ++i) {
        status = write(buffer_ptr[i]);
        if (status not_eq STREAM_WRITE_OK) {
            break;
        }
    }
    return status;
}
