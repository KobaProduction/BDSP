#include "vector_test_streams.h"

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
