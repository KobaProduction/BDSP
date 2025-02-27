#include <gtest/gtest.h>
#include <vector>

#include "BDSP/streams/types.h"
#include "BDSP/streams/PPP/reader.h"
#include "BDSP/streams/PPP/writer.h"

#include "testing.h"

using namespace BDSP::streams;
using namespace BDSP::streams::ppp;

TEST(ppp_integration_tests, read_test) {
    PPPWriterStream writer;
    PPPReaderStream reader;

    std::vector<uint8_t> data = {0x7E, 0x7D, 0x00};
    std::vector<uint8_t> correct_encoded = {0x7D, 0x5E, 0x7D, 0x5D, 0x00, 0x7E};

    start_test_writer(writer, data, correct_encoded, true);
    start_test_reader(reader, correct_encoded, data, true);
}

TEST(ppp_integration_tests, read_with_errors_test) {
    PPPReaderStream reader;
    std::vector<stream_read_status_t> statuses;

    reader.set_stream_data_handler(
        [](uint8_t byte, stream_read_status_t status, void *ctx) {
            reinterpret_cast<std::vector<stream_read_status_t> *>(ctx)->push_back(status);
        },
        &statuses);


    EXPECT_EQ(reader.read(0x7D), STREAM_READ_OK);
    ASSERT_TRUE(statuses.empty());

    EXPECT_EQ(reader.read(0x7E), STREAM_READ_DELIMITER);
    ASSERT_TRUE(statuses == std::vector<stream_read_status_t>({STREAM_READ_ERROR, STREAM_READ_DELIMITER}));

    statuses.clear();
    EXPECT_EQ(reader.read(0x7D), STREAM_READ_OK);
    ASSERT_TRUE(statuses.empty());

    EXPECT_EQ(reader.read(0x7E + 1), STREAM_READ_ERROR);
    ASSERT_TRUE(statuses == std::vector<stream_read_status_t>({STREAM_READ_ERROR}));
}
