#ifndef BDSP_TESTS_UTILS_VECTOR_TEST_STREAMS_H
#define BDSP_TESTS_UTILS_VECTOR_TEST_STREAMS_H

#include <vector>

#include "BDSP/streams/PPP/types.h"
#include "BDSP/streams/abstract/reader.h"
#include "BDSP/streams/abstract/writer.h"

class VectorTestWriter final: public BDSP::streams::ABS::AbstractWriter {
    void _finish() override;
    void _process_byte(uint8_t byte) override;
public:
    std::vector<std::vector<uint8_t>> stream_data;

    VectorTestWriter();
};

class VectorTestReader final: public BDSP::streams::IStreamReader {
public:
    BDSP::streams::stream_data_handler_t _data_handler = nullptr;
    void *_data_handler_context = nullptr;

    BDSP::streams::read_status_t read(uint8_t byte) override;
    BDSP::streams::read_status_t read(uint8_t *buffer_ptr, size_t size) override;
    void reset_read_state(bool need_wait_delimiter) override;
    void set_stream_data_handler(BDSP::streams::stream_data_handler_t handler, void *context_ptr) override;
    bool get_ready_status() override;

    bool ready_status = true;

    BDSP::streams::read_status_t read(uint8_t byte, BDSP::streams::read_status_t status);
};


#endif // BDSP_TESTS_UTILS_VECTOR_TEST_STREAMS_H
