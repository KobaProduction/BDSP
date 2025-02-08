#include <gtest/gtest.h>
#include <vector>

#include "BDSP/streams/PPP/checkers.h"
#include "BDSP/streams/PPP/reader.h"
#include "BDSP/streams/PPP/writer.h"

#include "../../utils/testing.h"

using namespace BDSP::streams;
using namespace BDSP::streams::PPP;
using namespace BDSP::streams::PPP::core;

TEST(ppp_pipelines_tests, set_config_checker_test) {
    ppp_config_t config = {0x7E, 0x7D, 0x20};
    EXPECT_EQ(check_ppp_config(config), SET_OK);

    config = {0x7E, 0x7D, 0x00};
    EXPECT_EQ(check_ppp_config(config), ERROR_ESCAPE_MASK_IS_NULL);

    config = {0x7E, 0x7E, 0x20};
    EXPECT_EQ(check_ppp_config(config), ERROR_END_AND_ESCAPE_BYTES_IS_EQUALS);

    config = {0x7E, 0x5E, 0x20};
    EXPECT_EQ(check_ppp_config(config), ERROR_INVERTED_END_AND_ESCAPE_BYTES_IS_EQUALS);
}

TEST(ppp_pipelines_tests, set_config_test) {
    PPPWriter writer;
    PPPReader reader;
    ppp_config_t config = writer.get_config();

    EXPECT_EQ(reader.set_config(config), SET_OK);
    EXPECT_EQ(writer.set_config(config), SET_OK);

    reader.set_stream_data_handler([](uint8_t byte, read_status_t status, void *ctx) { }, nullptr);

    EXPECT_EQ(reader.read(config.escape_byte), STREAM_READ_OK);
    EXPECT_EQ(reader.set_config(config), ERROR_PROCESS_NOT_FINISHED);
    reader.reset_read_state(false);

    config.escape_byte = config.end_byte;

    EXPECT_EQ(reader.set_config(config), ERROR_END_AND_ESCAPE_BYTES_IS_EQUALS);
    EXPECT_EQ(writer.set_config(config), ERROR_END_AND_ESCAPE_BYTES_IS_EQUALS);
}

TEST(ppp_pipelines_tests, encoding_test) {
    PPPWriter writer;
    PPPReader reader;

    std::vector<uint8_t> data = {0x7E, 0x7D, 0x00};
    std::vector<uint8_t> correct_encoded = {0x7D, 0x5E, 0x7D, 0x5D, 0x00, 0x7E};

    start_test_writer(writer, data, correct_encoded, true);
    start_test_reader(reader, correct_encoded, data, true);
}

TEST(ppp_pipelines_tests, decoding_errors_test) {
    PPPReader reader;
    ppp_config_t config = reader.get_config();

    std::vector<read_status_t> statuses;

    reader.set_stream_data_handler(
        [](uint8_t byte, read_status_t status, void *ctx) {
            reinterpret_cast<std::vector<read_status_t> *>(ctx)->push_back(status);
        },
        &statuses);

    EXPECT_EQ(reader.read(config.escape_byte), STREAM_READ_OK);
    ASSERT_TRUE(statuses.empty());

    EXPECT_EQ(reader.read(config.end_byte), STREAM_READ_END);
    ASSERT_TRUE(statuses == std::vector<read_status_t>({STREAM_READ_ERROR, STREAM_READ_END}));

    statuses.clear();
    EXPECT_EQ(reader.read(config.escape_byte), STREAM_READ_OK);
    ASSERT_TRUE(statuses.empty());

    EXPECT_EQ(reader.read(config.end_byte + 1), STREAM_READ_ERROR);
    ASSERT_TRUE(statuses == std::vector<read_status_t>({STREAM_READ_ERROR}));
}
