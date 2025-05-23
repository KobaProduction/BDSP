#include "BDSP/streams/abstract/reader.h"

using namespace BDSP::streams;
using namespace BDSP::streams::ABS;

void AbstractStreamReader::_handler(uint8_t byte, read_status_t status) {
    if (_is_waiting_for_the_delimiter) {
        return;
    }
    _data_handler(byte, status, _data_handler_context);
}

bool AbstractStreamReader::get_ready_status() {
    return _state == READY;
}

read_status_t AbstractStreamReader::read(uint8_t byte) {
    if (_state not_eq READY) {
        return STREAM_READER_NOT_READY_ERROR;
    }
    read_status_t status = _process_byte(byte);
    if (status not_eq STREAM_READ_OK) {
        _data_handler(byte, status, _data_handler_context);
        reset_read_state(status == STREAM_READ_ERROR);
    }
    return status;
}

read_status_t AbstractStreamReader::read(uint8_t *buffer_ptr, size_t size) {
    read_status_t status = read_status_t::STREAM_READ_OK;
    for (size_t i = 0; i < size; ++i) {
        switch (read(buffer_ptr[i])) {
        case STREAM_READER_NOT_READY_ERROR: return STREAM_READER_NOT_READY_ERROR;
        case STREAM_READ_ERROR: status = STREAM_READ_ERROR;
        default: break;
        }
    }
    return status;
}

void AbstractStreamReader::reset_read_state(bool is_need_wait_delimiter) {
    _is_waiting_for_the_delimiter = is_need_wait_delimiter;
    _reset();
}

void AbstractStreamReader::set_stream_data_handler(stream_data_handler_t handler, void *context_ptr) {
    _data_handler = handler;
    _data_handler_context = context_ptr;
    _set_handler_state(_data_handler not_eq nullptr);
}

