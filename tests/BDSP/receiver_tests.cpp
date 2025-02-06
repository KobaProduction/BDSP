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

TEST(bdsp_v1_receiver_tests, reset_destructor_test) {
    VectorTestReader reader;
    ASSERT_FALSE(reader.get_ready_status());
    {
        BDSPReceiver receiver;
        EXPECT_EQ(receiver.set_stream_reader(&reader), SET_STREAM_READER_OK);
        ASSERT_TRUE(reader.get_ready_status());
    };
    ASSERT_FALSE(reader.get_ready_status());
}

TEST(bdsp_v1_receiver_tests, error_parsing_test) {
    class BDSPReceiverTesting: public core::BDSPV1ReceiverCore {
    public:
        void set_fsm_state(core::receiver_fsm_state_t state) { _fsm_state = state; }
        void set_header(core::packet_v1_header header) { _packet_header = header; }

        void set_malloc(void *(new_malloc)(size_t)) { _malloc = new_malloc; }
    };

    BDSPReceiverTesting receiver;

    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_ERROR), STREAM_READING_ERROR);

    { // test error handler
        VectorTestReader reader;
        EXPECT_EQ(receiver.set_stream_reader(&reader), SET_STREAM_READER_OK);
        parse_packet_status_t current_error = PARSE_PACKET_BYTE_OK;
        receiver.set_error_handler(
            [](parse_packet_status_t error, void *ctx) { *reinterpret_cast<parse_packet_status_t *>(ctx) = error; },
            &current_error);
        EXPECT_EQ(reader.read(0x00, STREAM_READ_ERROR), STREAM_READ_ERROR);
        EXPECT_EQ(current_error, STREAM_READING_ERROR);
        receiver.set_error_handler(nullptr, nullptr);
        EXPECT_EQ(receiver.set_stream_reader(nullptr), RESET_STREAM_READER);
    }

    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_ERROR), STREAM_READING_ERROR);
    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READER_NOT_READY_ERROR), STREAM_READING_ERROR);
    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_END), STREAM_READING_ERROR);

    core::packet_v1_header header{};
    header.is_unsupported_protocol_version = true;
    EXPECT_EQ(receiver.parse_packet_byte(*reinterpret_cast<uint8_t *>(&header), STREAM_READ_OK),
              UNSUPPORTED_PROTOCOL_ERROR);

    header.is_unsupported_protocol_version = false;
    header.is_two_bytes_for_packet_size = false;

    receiver.set_fsm_state(core::PACKET_SIZE_A);
    receiver.set_header(header);
    EXPECT_EQ(receiver.parse_packet_byte(0, STREAM_READ_OK), EXCEEDING_THE_MAXIMUM_PACKET_SIZE_ERROR);

    receiver.set_fsm_state(core::PACKET_SIZE_A);
    receiver.set_max_packet_size(200);
    EXPECT_EQ(receiver.parse_packet_byte(255, STREAM_READ_OK), EXCEEDING_THE_MAXIMUM_PACKET_SIZE_ERROR);

    receiver.set_fsm_state(core::PACKET_SIZE_A);
    receiver.set_malloc([](size_t size) -> void * { return nullptr; });
    EXPECT_EQ(receiver.parse_packet_byte(10, STREAM_READ_OK), NOT_ENOUGH_RAM_FOR_PACKET_ERROR);

    receiver.set_fsm_state(core::WAIT_END);
    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_OK), STREAM_READING_ERROR);

    receiver.set_fsm_state(core::WAIT_END);
    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_END), PACKET_HANDLER_NOT_SET_ERROR);
}

TEST(bdsp_v1_receiver_tests, checksum_error_test) {
    class ChecksumTester: public core::BDSPV1ChecksumMixin {
    public:
        uint8_t calc(core::packet_v1_header header, uint8_t *data_ptr, uint16_t size) {
            return _calc_checksum(header, data_ptr, size);
        }
    };

    BDSPReceiver receiver;
    ChecksumTester checksum_tester;

    receiver.set_checksum_function(checksums::crc8_nrsc5);
    checksum_tester.set_checksum_function(checksums::crc8_nrsc5);

    core::packet_v1_header header = {.packet_id = 0,
                                     .is_two_bytes_for_packet_size = false,
                                     .is_checksum_used = true,
                                     .is_unsupported_protocol_version = false};

    std::vector<uint8_t> data = {99};
    EXPECT_EQ(receiver.parse_packet_byte(*reinterpret_cast<uint8_t *>(&header), STREAM_READ_OK), STREAM_READ_OK);
    EXPECT_EQ(receiver.parse_packet_byte(data.size(), STREAM_READ_OK), STREAM_READ_OK); // data size
    for (auto byte: data) {
        EXPECT_EQ(receiver.parse_packet_byte(byte, STREAM_READ_OK), STREAM_READ_OK); // data
    }
    uint8_t correct_checksum = checksum_tester.calc(header, data.data(), data.size());
    EXPECT_EQ(receiver.parse_packet_byte(correct_checksum + 1, STREAM_READ_OK), CHECKSUM_ERROR); // data
}