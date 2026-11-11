#include <gtest/gtest.h>

#include "BDSP/transmitter.h"

#include "testing.h"
#include "vector_test_streams.h"

using namespace BDSP;
using namespace BDSP::core;
using namespace BDSP::streams;

TEST(bdsp_v1_transmitter_tests, set_stream_writer_test) {
    VectorTestWriter writer;
    TransmitterV1Core transmitter;

    writer.set_stream_writer(nullptr, nullptr);
    ASSERT_FALSE(writer.get_ready_status());
    EXPECT_EQ(transmitter.set_stream_writer(&writer), STREAM_WRITER_NOT_READY);

    writer.set_stream_writer([](uint8_t byte, void *ctx) { }, nullptr);
    ASSERT_TRUE(writer.get_ready_status());
    EXPECT_EQ(transmitter.set_stream_writer(&writer), SET_STREAM_WRITER_OK);

    EXPECT_EQ(transmitter.set_stream_writer(nullptr), RESET_STREAM_WRITER);
}

TEST(bdsp_v1_transmitter_tests, error_send_data_test) {
    struct Context {
        VectorTestWriter writer;
        TransmitterV1Core transmitter;
    } ctx;

    uint8_t packet_id = 31;

    std::vector<uint8_t> data;

    ctx.writer.set_stream_writer([](uint8_t byte, void *ctx) { }, nullptr);

    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size(), false), STREAM_WRITER_NOT_SET_ERROR);
    EXPECT_EQ(ctx.transmitter.set_stream_writer(&ctx.writer), SET_STREAM_WRITER_OK);
    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size(), false),
              MAXIMUM_PACKET_SIZE_EXCEEDING_ERROR);

    ctx.transmitter.set_max_packet_size(127);
    for (int i = 0; i < 255; ++i) {
        data.push_back(i);
    }
    EXPECT_EQ(ctx.transmitter.send_data(packet_id, data.data(), data.size(), false),
              MAXIMUM_PACKET_SIZE_EXCEEDING_ERROR);

    ctx.transmitter.set_max_packet_size(65535);
    EXPECT_EQ(ctx.transmitter.send_data(255, data.data(), data.size(), false), PACKET_ID_ERROR);
}

TEST(bdsp_v1_transmitter_tests, send_data_test) {
    Transmitter<VectorTestWriter> transmitter;
    transmitter.get_write_stream().set_stream_writer([](uint8_t byte, void *ctx) { }, nullptr);

    uint8_t packet_id = 0;
    std::vector<uint8_t> data = {99};
    uint8_t buffer = data.size();
    std::vector<uint8_t> correct = {0, buffer};
    for (uint8_t &i: data) {
        correct.push_back(i);
    }

    EXPECT_EQ(transmitter.send_data(packet_id, data.data(), data.size(), false), SEND_PACKET_OK);
    ASSERT_FALSE(transmitter.get_write_stream().get_strategy().stream_data.empty());
    EXPECT_EQ(transmitter.get_write_stream().get_strategy().stream_data.size(), 2);

    check_data_for_correctness(transmitter.get_write_stream().get_strategy().stream_data[0], correct);

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

    EXPECT_EQ(transmitter.send_data(packet_id, data.data(), data.size(), false), SEND_PACKET_OK);
    EXPECT_EQ(transmitter.get_write_stream().get_strategy().stream_data.size(), 3);

    check_data_for_correctness(transmitter.get_write_stream().get_strategy().stream_data[1], correct);
}

TEST(bdsp_v1_transmitter_tests, checksums_test) {
    Transmitter<VectorTestWriter> transmitter;
    transmitter.get_write_stream().set_stream_writer([](uint8_t byte, void *ctx) { }, nullptr);

    EXPECT_EQ(transmitter.set_checksum_function(checksums::crc8_nrsc5), SET_CHECKSUM_FUNCTION_OK);

    std::vector<uint8_t> _data = {99};
    std::vector<uint8_t> correct_without_checksum = {0, 1, 99};
    std::vector<uint8_t> correct_with_checksum = {64, 1, 99, 165};

    packet_context_t packet_context = {0, _data.data(), uint16_t(_data.size())};

    transmitter.set_checksum_usage_default_state(false);
    EXPECT_EQ(transmitter.send_data(packet_context.packet_id, packet_context.data_ptr, packet_context.size),
              SEND_PACKET_OK);
    check_data_for_correctness( transmitter.get_write_stream().get_strategy().stream_data[0], correct_without_checksum);

    EXPECT_EQ(transmitter.send_packet(packet_context), SEND_PACKET_OK);
    check_data_for_correctness( transmitter.get_write_stream().get_strategy().stream_data[1], correct_without_checksum);
    EXPECT_EQ(transmitter.send_data(packet_context.packet_id, packet_context.data_ptr, packet_context.size, false),
              SEND_PACKET_OK);
    check_data_for_correctness( transmitter.get_write_stream().get_strategy().stream_data[1], correct_without_checksum);

    EXPECT_EQ(transmitter.send_data(packet_context.packet_id, packet_context.data_ptr, packet_context.size, true),
              SEND_PACKET_OK);
    check_data_for_correctness( transmitter.get_write_stream().get_strategy().stream_data[3], correct_with_checksum);
    transmitter.set_checksum_usage_default_state(true);
    EXPECT_EQ(transmitter.send_packet(packet_context), SEND_PACKET_OK);
    check_data_for_correctness( transmitter.get_write_stream().get_strategy().stream_data[4], correct_with_checksum);
    EXPECT_EQ(transmitter.send_data(packet_context.packet_id, packet_context.data_ptr, packet_context.size),
              SEND_PACKET_OK);
    check_data_for_correctness( transmitter.get_write_stream().get_strategy().stream_data[5], correct_with_checksum);
}

TEST(bdsp_v1_transmitter_tests, dealocate_memory_after_send) {
    Transmitter<VectorTestWriter> transmitter;
    transmitter.get_write_stream().set_stream_writer([](uint8_t byte, void *ctx) { }, nullptr);

    auto data = reinterpret_cast<uint8_t *>(malloc(1));
    *data = 99;
    std::vector<uint8_t> correct_with_checksum = {0, 1, 99};

    packet_context_t packet_context = {0, data, 1, true};

    EXPECT_EQ(transmitter.send_packet(packet_context, false), SEND_PACKET_OK);
    check_data_for_correctness(transmitter.get_write_stream().get_strategy().stream_data[0], correct_with_checksum);
    EXPECT_EQ(packet_context.data_ptr, nullptr);
}

TEST(bdsp_v1_transmitter_tests, reject_oversized_and_null_payloads) {
    Transmitter<VectorTestWriter> transmitter;
    transmitter.get_write_stream().set_stream_writer([](uint8_t, void *) {}, nullptr);

    uint8_t payload = 0x42;
    EXPECT_EQ(transmitter.send_data(1, &payload, size_t{65536}, false),
              MAXIMUM_PACKET_SIZE_EXCEEDING_ERROR);
    EXPECT_EQ(transmitter.send_data(1, &payload, size_t{65537}, false),
              MAXIMUM_PACKET_SIZE_EXCEEDING_ERROR);
    EXPECT_EQ(transmitter.send_data(1, nullptr, size_t{1}, false),
              PACKET_DATA_NULL_POINTER_ERROR);
    EXPECT_EQ(transmitter.send_data(1, nullptr, size_t{0}, false),
              MAXIMUM_PACKET_SIZE_EXCEEDING_ERROR);
}

TEST(bdsp_v1_transmitter_tests, reject_stream_writer_after_it_becomes_unready) {
    Transmitter<VectorTestWriter> transmitter;
    uint8_t payload = 0x42;
    auto &writer = transmitter.get_write_stream();
    writer.set_stream_writer([](uint8_t, void *) {}, nullptr);
    ASSERT_TRUE(writer.get_ready_status());
    writer.set_stream_writer(nullptr, nullptr);
    ASSERT_FALSE(writer.get_ready_status());
    EXPECT_EQ(transmitter.send_data(0, &payload, 1, false),
              STREAM_WRITER_NOT_READY_SEND_ERROR);
}


TEST(bdsp_v1_transmitter_tests, report_writer_failure_during_packet) {
    // The writer may lose readiness after the first output operation. A partial
    // frame must not be reported as a successful packet transmission.
    class FailingWriter final: public IStreamWriter {
    public:
        int attempts = 0;
        bool ready = true;
        stream_write_status_t finish() override {
            return ready ? STREAM_WRITE_DELIMITER : STREAM_WRITER_NOT_READY_ERROR;
        }
        void set_stream_writer(stream_write_handler_t, void *) override {}
        stream_write_status_t write(uint8_t) override {
            return write(static_cast<uint8_t *>(nullptr), 0);
        }
        stream_write_status_t write(uint8_t *, size_t) override {
            ++attempts;
            if (not ready) return STREAM_WRITER_NOT_READY_ERROR;
            ready = false;
            return STREAM_WRITE_OK;
        }
        bool get_ready_status() override { return ready; }
    } writer;

    TransmitterV1Core transmitter;
    ASSERT_EQ(transmitter.set_stream_writer(&writer), SET_STREAM_WRITER_OK);
    uint8_t byte = 42;
    EXPECT_EQ(transmitter.send_data(3, &byte, 1, false), STREAM_WRITER_NOT_READY_SEND_ERROR);
    EXPECT_EQ(writer.attempts, 2);
}
