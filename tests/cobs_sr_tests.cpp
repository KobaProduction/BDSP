#include <gtest/gtest.h>

#include <BDSP/streams/COBS/reader.h>
#include <BDSP/streams/COBS/writer.h>

#include "utils/cobs.h"
#include "utils/show.h"
#include "utils/testing.h"

using namespace BDSP::streams;
using namespace BDSP::streams::COBS;
using namespace BDSP::streams::COBS::core;

TEST(cobs_sr_tests, cobs_sr_set_configuration_test) {
    class COBSSRWriterCoreTest: public COBSSRWriterCore {
    public:
        void set_current_size_of_the_sequence_to_be_replaced(uint8_t value) {
            _current_size_of_the_sequence_to_be_replaced = value;
        }
    };
    COBSSRWriterCoreTest cobs_writer;
    COBSSRReader cobs_reader;

    cobs_config_t config = {.delimiter = '\0',
                            .depth = 127,
                            .size_of_the_sequence_to_be_replaced = 2,
                            .byte_of_the_sequence_to_be_replaced = '\0'};

    EXPECT_EQ(cobs_writer.set_config(config), SET_OK);

    cobs_writer.set_current_size_of_the_sequence_to_be_replaced(1);
    EXPECT_EQ(cobs_writer.set_config(config), ERROR_PROCESS_NOT_FINISHED);
    cobs_writer.set_current_size_of_the_sequence_to_be_replaced(0);

    config.depth = 255;
    EXPECT_EQ(cobs_writer.set_config(config), ERROR_DEPTH_SR);
    EXPECT_EQ(cobs_reader.set_config(config), ERROR_DEPTH_SR);

    config.depth = 1;
    EXPECT_EQ(cobs_reader.set_config(config), ERROR_COBS_DEPTH);
}

TEST(cobs_sr_tests, check_reader_errors_test) {
    cobs_config_t config = {.delimiter = 0, .depth = 127, .size_of_the_sequence_to_be_replaced = 2};

    COBSSRReader cobs_reader;

    EXPECT_EQ(cobs_reader.set_config(config), SET_OK);

    //
    cobs_reader.set_stream_data_handler([](uint8_t byte, read_status_t status, void *ctx) { }, nullptr);

    EXPECT_EQ(cobs_reader.read(128), STREAM_READ_OK);
    EXPECT_EQ(cobs_reader.read(config.delimiter), STREAM_READ_ERROR);
    cobs_reader.reset_read_state(false);

    EXPECT_EQ(cobs_reader.read(128), STREAM_READ_OK);
    EXPECT_EQ(cobs_reader.read(config.depth * 2 + 1), STREAM_READ_ERROR);
}

TEST(cobs_sr_tests, writer_reset_elimination_sequence_test) {
    COBSSRWriter cobs_writer;
    COBSSRReader cobs_reader;

    cobs_config_t config = cobs_writer.get_config();
    config.delimiter = 255;
    cobs_writer.set_config(config);
    cobs_reader.set_config(config);

    std::vector<uint8_t> data = {config.byte_of_the_sequence_to_be_replaced};
    std::vector<uint8_t> correct_encoded = {2, config.byte_of_the_sequence_to_be_replaced, config.delimiter};
    start_test_writer(cobs_writer, data, correct_encoded);
    start_test_reader(cobs_reader, correct_encoded, data);

    data = {config.byte_of_the_sequence_to_be_replaced, 1};
    correct_encoded = {3, config.byte_of_the_sequence_to_be_replaced, 1, config.delimiter};
    start_test_writer(cobs_writer, data, correct_encoded);
    start_test_reader(cobs_reader, correct_encoded, data);

    data = {1, 1};
    correct_encoded = {3, 1, 1, config.delimiter};
    start_test_writer(cobs_writer, data, correct_encoded);
    start_test_reader(cobs_reader, correct_encoded, data);
}

TEST(cobs_sr_tests, sequence_replacement_test) {
    COBSSRWriter cobs_writer;
    COBSSRReader cobs_reader;

    std::vector<uint8_t> data = {0x01, 0x00, 0x00, 0x00, 0x01};
    std::vector<uint8_t> correct_encoded = {129, 0x01, 0x01, 0x02, 0x01, 0x00};
    start_test_writer(cobs_writer, data, correct_encoded);
    start_test_reader(cobs_reader, correct_encoded, data);
}
