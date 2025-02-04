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


bool VectorTestWriter::is_empty() {
    if (stream_data.empty() or (stream_data.size() == 1 and stream_data[0].empty())) {
        return true;
    }
    return false;
}

VectorTestWriter::VectorTestWriter() {
    set_stream_writer([] (uint8_t byte, void *ctx) {}, nullptr);
}
