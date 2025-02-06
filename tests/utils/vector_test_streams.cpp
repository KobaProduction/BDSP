#include "vector_test_streams.h"

using namespace BDSP::streams;

void VectorTestWriter::_process_byte(uint8_t byte) {
    if (stream_data.empty()) {
        stream_data.emplace_back();

    }
    stream_data.back().push_back(byte);
}

void VectorTestWriter::_finish() {
    if (stream_data.empty() or stream_data.back().empty()) {
        return;
    }
    stream_data.emplace_back();
}

VectorTestWriter::VectorTestWriter() {
    set_stream_writer([] (uint8_t byte, void *ctx) {}, nullptr);
}

read_status_t VectorTestReader::read(uint8_t byte, read_status_t status) {
    if (not _data_handler) return STREAM_READER_NOT_READY_ERROR;
    _data_handler(byte, status, _data_handler_context);
    return status;
}

read_status_t VectorTestReader::read(uint8_t byte) {
    if (not _data_handler) return STREAM_READER_NOT_READY_ERROR;
    _data_handler(byte, STREAM_READ_OK, _data_handler_context);
    return STREAM_READ_OK;
}

read_status_t VectorTestReader::read(uint8_t *buffer_ptr, size_t size) {
    for (int i = 0; i < size; ++i) {
        read(buffer_ptr[i], STREAM_READ_OK);
    }
    return STREAM_READ_OK;
}

void VectorTestReader::reset_read_state(bool need_wait_delimiter) {

}

void VectorTestReader::set_stream_data_handler(stream_data_handler_t handler, void *context_ptr) {
    _data_handler = handler;
    _data_handler_context = context_ptr;
}

bool VectorTestReader::get_ready_status() {
    return bool(_data_handler);
}
