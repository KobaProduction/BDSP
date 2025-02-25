#include <gtest/gtest.h>

#include "BDSP/streams/cobs/reader.h"
#include "BDSP/streams/cobs/writer.h"

#include "../../../../utils/testing.h"

using namespace BDSP::streams::cobs;
using namespace BDSP::streams::strategies;
using namespace BDSP::streams::strategies::cobs;

TEST(strategies_strategies_cobs_zpe_tests, writer_reset_elimination_sequence_test) {
    COBSZPEWriterStream writer;
    cobs_config_t config = writer.get_strategy().get_config();

    std::vector<uint8_t> data = {config.byte_of_the_sequence_to_be_replaced, 1};
    std::vector<uint8_t> correct_encoded = {1, 2, 1, config.delimiter_byte};
    start_test_writer(writer, data, correct_encoded);
}

TEST(strategies_strategies_cobs_zpe_tests, writer_elimination_sequence_after_offset_test) {
    COBSZPEWriterStream writer;
    cobs_config_t config = writer.get_strategy().get_config();
    config.delimiter_byte = 255;
    EXPECT_EQ(writer.get_strategy().set_config(config), SET_OK);

    writer.set_stream_writer([](uint8_t byte, void *ctx_ptr) { }, nullptr);

    std::vector<uint8_t> data;

    data.push_back(config.byte_of_the_sequence_to_be_replaced);

    for (int i = 0; i < 255 - config.depth; ++i) {
        data.push_back(99);
    }
    data.push_back(config.byte_of_the_sequence_to_be_replaced);
    std::vector<uint8_t> correct_encoded = {34,
                                            config.byte_of_the_sequence_to_be_replaced,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            99,
                                            config.byte_of_the_sequence_to_be_replaced,
                                            config.delimiter_byte};
    start_test_writer(writer, data, correct_encoded);
}

TEST(strategies_strategies_cobs_zpe_tests, writer_elimination_sequence_test) {
    COBSZPEWriterStream writer;
    cobs_config_t config = writer.get_strategy().get_config();

    std::vector<uint8_t> data = {config.byte_of_the_sequence_to_be_replaced, config.byte_of_the_sequence_to_be_replaced};
    std::vector<uint8_t> correct_encoded = {0xE1, 1, config.delimiter_byte};
    start_test_writer(writer, data, correct_encoded);
}
