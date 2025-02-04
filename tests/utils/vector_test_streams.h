#ifndef BDSP_TESTS_UTILS_VECTOR_TEST_STREAMS_H
#define BDSP_TESTS_UTILS_VECTOR_TEST_STREAMS_H

#include <vector>

#include "BDSP/streams/PPP/types.h"
#include "BDSP/streams/abstract/writer.h"

using namespace BDSP::streams;

class VectorTestWriter final: public ABS::AbstractWriter {
    void _finish() override;
    void _process_byte(uint8_t byte) override;
public:
    std::vector<std::vector<uint8_t>> stream_data;

    VectorTestWriter();
};


#endif // BDSP_TESTS_UTILS_VECTOR_TEST_STREAMS_H
