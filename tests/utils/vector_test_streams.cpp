#include "vector_test_streams.h"

using namespace BDSP::streams::strategies;

void TestVectorWriteStrategy::write(uint8_t byte) {
    if (stream_data.empty()) {
        stream_data.emplace_back();
    }
    stream_data.back().push_back(byte);
}

void TestVectorWriteStrategy::send_delimiter() {
    stream_data.emplace_back();
}

void TestVectorWriteStrategy::finish() {
    if (stream_data.empty() or stream_data.back().empty()) {
        return;
    }
    send_delimiter();
}

void TestVectorReadStrategy::reset_read_state() { }

strategy_read_status_t TestVectorReadStrategy::read(uint8_t byte) {
    _data_callback(byte, STRATEGY_READ_OK, _context);
    return STRATEGY_READ_OK;
}

strategy_read_status_t TestVectorReadStrategy::read_with_status(uint8_t byte,
    BDSP::streams::strategies::strategy_read_status_t status) {
    _data_callback(byte, status, _context);
    return status;
}
