#include <gtest/gtest.h>

#include "BDSP/streams/COBS/reader.h"
#include "BDSP/streams/COBS/writer.h"

#include "../../utils/cobs.h"
#include "../../utils/show.h"
#include "../../utils/testing.h"

using namespace BDSP::streams;
using namespace BDSP::streams::COBS;
using namespace BDSP::streams::COBS::core;

TEST(cobs_default_tests, cobs_default_set_configuration_test) {
    COBSWriter cobs_writer;
    COBSReader cobs_reader;

    cobs_config_t config = {.delimiter = '\0',
                            .depth = 255,
                            .size_of_the_sequence_to_be_replaced = 0,
                            .byte_of_the_sequence_to_be_replaced = '\0'};

    EXPECT_EQ(cobs_writer.set_config(config), SET_OK);
    config.size_of_the_sequence_to_be_replaced = 2;
    EXPECT_EQ(cobs_writer.set_config(config), ERROR_DEFAULT_COBS_SIZE_SR);
    EXPECT_EQ(cobs_reader.set_config(config), ERROR_DEFAULT_COBS_SIZE_SR);
}

TEST(cobs_default_tests, reader_errors_test) {
    COBSReader reader;
    cobs_config_t config = reader.get_config();

    std::vector<read_status_t> statuses;

    reader.set_stream_data_handler(
        [](uint8_t byte, read_status_t status, void *ctx) {
            reinterpret_cast<std::vector<read_status_t> *>(ctx)->push_back(status);
        },
        &statuses);

    EXPECT_EQ(reader.read(config.delimiter + 2), STREAM_READ_OK);
    ASSERT_TRUE(statuses.empty());

    EXPECT_EQ(reader.read(config.delimiter), STREAM_READ_END);
    ASSERT_TRUE(statuses == std::vector<read_status_t>({STREAM_READ_ERROR, STREAM_READ_END}));

    config.delimiter = 1;
    config.depth = 16;

    EXPECT_EQ(reader.set_config(config), SET_OK);

    statuses.clear();
    EXPECT_EQ(reader.read(0), STREAM_READ_OK);
    EXPECT_EQ(reader.read(255), STREAM_READ_ERROR);
    ASSERT_TRUE(statuses == std::vector<read_status_t>({STREAM_READ_ERROR}));
}

TEST(cobs_default_tests, encoding_depth_test) {
    COBSWriter cobs_writer;
    COBSReader cobs_reader;
    auto config = cobs_writer.get_config();
    config.depth = 16;
    EXPECT_EQ(cobs_reader.set_config(config), SET_OK);
    EXPECT_EQ(cobs_writer.set_config(config), SET_OK);

    std::vector<uint8_t> data;
    std::vector<uint8_t> correct_encoded = {
        config.depth, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, config.delimiter};

    for (int i = 0; i < config.depth - 1; ++i) {
        data.push_back(255);
    }

    start_test_writer(cobs_writer, data, correct_encoded);
    start_test_reader(cobs_reader, correct_encoded, data);

    data.push_back(255);

    correct_encoded = {config.depth,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       2,
                       255,
                       config.delimiter};

    start_test_writer(cobs_writer, data, correct_encoded);
    start_test_reader(cobs_reader, correct_encoded, data);
}

TEST(cobs_default_tests, encoding_custom_delimiter_test) {
    COBSWriter cobs_writer;
    COBSReader cobs_reader;
    auto config = cobs_writer.get_config();
    config.delimiter = 2;
    EXPECT_EQ(cobs_reader.set_config(config), SET_OK);
    EXPECT_EQ(cobs_writer.set_config(config), SET_OK);

    std::vector<uint8_t> data = {0, 2, 0, 0};
    std::vector<uint8_t> correct_encoded = {0, 0, 3, 0, 0, 2};

    start_test_writer(cobs_writer, data, correct_encoded);
    start_test_reader(cobs_reader, correct_encoded, data);
}