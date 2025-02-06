#include <gtest/gtest.h>

#include "BDSP/transmitter.h"

#include "../utils/testing.h"
#include "../utils/vector_test_streams.h"

using namespace BDSP;
using namespace BDSP::streams;

TEST(bdsp_v1_transmitter_tests, set_stream_writer_test) {
    VectorTestWriter writer;
    BDSPTransmitter transmitter;

    writer.set_stream_writer(nullptr, nullptr);
    ASSERT_FALSE(writer.get_ready_status());
    EXPECT_EQ(transmitter.set_stream_writer(&writer), STREAM_WRITER_NOT_READY_ERROR);

    writer.set_stream_writer([](uint8_t byte, void *ctx) { }, nullptr);
    ASSERT_TRUE(writer.get_ready_status());
    EXPECT_EQ(transmitter.set_stream_writer(&writer), SET_STREAM_WRITER_OK);

    EXPECT_EQ(transmitter.set_stream_writer(nullptr), RESET_STREAM_WRITER);
}

TEST(bdsp_v1_transmitter_tests, error_send_data_test) {
    struct Context {
        VectorTestWriter writer;
        BDSPTransmitter transmitter;
    } ctx;

    uint8_t packet_id = 31;

    std::vector<uint8_t> data;

    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size(), WITHOUT_CHECKSUM), STREAM_WRITER_NOT_SET_ERROR);
    EXPECT_EQ(ctx.transmitter.set_stream_writer(&ctx.writer), SET_STREAM_WRITER_OK);
    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size(), WITHOUT_CHECKSUM), MAXIMUM_PACKET_SIZE_EXCEEDING_ERROR);

    ctx.transmitter.set_max_packet_size(127);
    for (int i = 0; i < 255; ++i) {
        data.push_back(i);
    }
    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size(), WITHOUT_CHECKSUM), MAXIMUM_PACKET_SIZE_EXCEEDING_ERROR);

    ctx.transmitter.set_max_packet_size(65535);
    EXPECT_EQ(ctx.transmitter.send_data(255, data.data(), data.size(), WITHOUT_CHECKSUM), PACKET_ID_ERROR);
}

TEST(bdsp_v1_transmitter_tests, send_data_test) {
    struct Context {
        VectorTestWriter writer;
        BDSPTransmitter transmitter;
    } ctx;

    EXPECT_EQ(ctx.transmitter.set_stream_writer(&ctx.writer), SET_STREAM_WRITER_OK);

    uint8_t packet_id = 0;
    std::vector<uint8_t> data = {99};
    uint8_t buffer = data.size();
    std::vector<uint8_t> correct = {0, buffer};
    for (uint8_t &i: data) {
        correct.push_back(i);
    }

    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size(), WITHOUT_CHECKSUM), SEND_PACKET_OK);
    ASSERT_FALSE(ctx.writer.stream_data.empty());
    EXPECT_EQ(ctx.writer.stream_data.size(), 2);

    check_data_for_correctness(ctx.writer.stream_data[0], correct);

    for (int i = 0; i < 300; ++i) {
        data.push_back(88);
    }
    buffer = packet_id + 32; // packet_id + two bytes flag bit
    correct = {buffer};
    buffer = data.size() >> 8;
    correct.push_back(buffer);
    buffer = data.size();
    correct.push_back(buffer);
    for (uint8_t &i: data) {
        correct.push_back(i);
    }

    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size(), WITHOUT_CHECKSUM), SEND_PACKET_OK);
    EXPECT_EQ(ctx.writer.stream_data.size(), 3);

    check_data_for_correctness(ctx.writer.stream_data[1], correct);
}

TEST(bdsp_v1_transmitter_tests, checksums_test) {
    struct Context {
        VectorTestWriter writer;
        BDSPTransmitter transmitter;
    } ctx;

    EXPECT_EQ(ctx.transmitter.set_stream_writer(&ctx.writer), SET_STREAM_WRITER_OK);
    EXPECT_EQ(ctx.transmitter.set_checksum_function(checksums::crc8_nrsc5), SET_CHECKSUM_FUNCTION_OK);

    uint8_t packet_id = 0;
    std::vector<uint8_t> data = {99};
    std::vector<uint8_t> correct = {0, 1, 99};

    ctx.transmitter.set_checksum_usage_default_state(false);
    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size()), SEND_PACKET_OK);
    check_data_for_correctness(ctx.writer.stream_data[0], correct);
    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size(), WITHOUT_CHECKSUM), SEND_PACKET_OK);
    check_data_for_correctness(ctx.writer.stream_data[1], correct);

    correct = {64, 1, 99, 165};
    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size(), WITH_CHECKSUM), SEND_PACKET_OK);
    check_data_for_correctness(ctx.writer.stream_data[2], correct);
    ctx.transmitter.set_checksum_usage_default_state(true);
    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size()), SEND_PACKET_OK);
    check_data_for_correctness(ctx.writer.stream_data[3], correct);
}