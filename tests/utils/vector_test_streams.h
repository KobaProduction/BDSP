#ifndef BDSP_TESTS_UTILS_VECTOR_TEST_STREAMS_H
#define BDSP_TESTS_UTILS_VECTOR_TEST_STREAMS_H

#include <vector>

#include "BDSP/strategies/abstract/read.h"
#include "BDSP/strategies/abstract/write.h"
#include "BDSP/strategies/byte_stuffing/types.h"
#include "BDSP/streams/reader.h"
#include "BDSP/streams/writer.h"

class TestVectorWriteStrategy final: public BDSP::strategies::abstract::AbstractWriteStrategy {
    void _init() override { };

public:
    std::vector<std::vector<uint8_t>> stream_data;

    void send_delimiter() override;
    void finish() override;
    void write(uint8_t byte) override;
};

class VectorTestWriter final: public BDSP::streams::StreamWriter<TestVectorWriteStrategy> { };

class TestVectorReadStrategy final: public BDSP::strategies::abstract::AbstractReadStrategy {
public:
    void _init() override { };

    BDSP::strategies::strategy_read_status_t read(uint8_t byte) override;
    BDSP::strategies::strategy_read_status_t read_with_status(uint8_t byte,
                                                              BDSP::strategies::strategy_read_status_t status);
    void reset_read_state() override;
};

class VectorTestReader final: public BDSP::streams::StreamReader<TestVectorReadStrategy> { };

#endif // BDSP_TESTS_UTILS_VECTOR_TEST_STREAMS_H
