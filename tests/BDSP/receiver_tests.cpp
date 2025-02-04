#include <gtest/gtest.h>

#include "BDSP/receiver.h"

#include "../utils/testing.h"
#include "../utils/vector_test_streams.h"

using namespace BDSP;
using namespace BDSP::streams;

TEST(bdsp_v1_receiver_tests, set_stream_reader_test) {
    VectorTestReader reader;
    BDSPReceiver receiver;

    reader.set_stream_data_handler([](uint8_t byte, read_status_t status, void *ctx) { }, nullptr);

    EXPECT_EQ(receiver.set_stream_reader(&reader), STREAM_READER_ALREADY_USED_ERROR);

    EXPECT_EQ(receiver.set_stream_reader(nullptr), STREAM_READER_NULL_POINTER_ERROR);

    reader.set_stream_data_handler(nullptr, nullptr);
    EXPECT_EQ(receiver.set_stream_reader(&reader), SET_STREAM_READER_OK);

    VectorTestReader reader2;
    EXPECT_EQ(receiver.set_stream_reader(&reader2), CHANGE_STREAM_READER);
    EXPECT_EQ(receiver.set_stream_reader(nullptr), RESET_STREAM_READER);
}
