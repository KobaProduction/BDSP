#include <gtest/gtest.h>

#include "BDSP/receiver.h"

#include "testing.h"
#include "vector_test_streams.h"

using namespace BDSP;
using namespace BDSP::streams;
using namespace BDSP::strategies;

TEST(bdsp_v1_receiver_tests, set_stream_reader_test) {
    VectorTestReader reader;
    BDSPReceiver receiver;

    reader.set_stream_data_handler([](uint8_t byte, stream_read_status_t status, void *ctx) { }, nullptr);

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

TEST(bdsp_v1_receiver_tests, parsing_statuses_test) {
    BDSPReceiver receiver;

    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_ERROR), STREAM_READING_ERROR);
    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READER_NOT_READY_ERROR), STREAM_READING_ERROR);
    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_DELIMITER), STREAM_READING_ERROR);
}

TEST(bdsp_v1_receiver_tests, error_parsing_test) {
    BDSPReceiver receiver;

    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_ERROR), STREAM_READING_ERROR);

    { // test error handler
        VectorTestReader reader;
        auto read_strategy = reader.get_strategy();
        EXPECT_EQ(receiver.set_stream_reader(&reader), SET_STREAM_READER_OK);
        parse_packet_status_t current_error = PARSE_PACKET_BYTE_OK;
        receiver.set_error_handler(
            [](parse_packet_status_t error, void *ctx) { *reinterpret_cast<parse_packet_status_t *>(ctx) = error; },
            &current_error);
        EXPECT_EQ(read_strategy.read_with_status(0x00, STRATEGY_READ_ERROR), STREAM_READ_ERROR);
        EXPECT_EQ(current_error, STREAM_READING_ERROR);
        receiver.set_error_handler(nullptr, nullptr);
        EXPECT_EQ(receiver.set_stream_reader(nullptr), RESET_STREAM_READER);
    }

    BDSP::core::packet_v1_header header{};
    header.is_unsupported_protocol_version = true;
    EXPECT_EQ(receiver.parse_packet_byte(*reinterpret_cast<uint8_t *>(&header), STREAM_READ_OK),
              UNSUPPORTED_PROTOCOL_ERROR);

    header.is_unsupported_protocol_version = false;
    header.is_two_bytes_for_packet_size = false;

    class AccessFieldsClass: public BDSP::core::BDSPV1ReceiverCore {
    public:
        using BDSP::core::BDSPV1ReceiverCore::receiver_fsm_state_t;
        using BDSP::core::BDSPV1ReceiverCore::_fsm_state;
        using BDSP::core::BDSPV1ReceiverCore::_packet_header;
        using BDSP::core::BDSPV1ReceiverCore::_malloc;
    };
    reinterpret_cast<AccessFieldsClass *>(&receiver)->_fsm_state =
        AccessFieldsClass::receiver_fsm_state_t::PACKET_SIZE_A;
    reinterpret_cast<AccessFieldsClass *>(&receiver)->_packet_header = header;
    EXPECT_EQ(receiver.parse_packet_byte(0, STREAM_READ_OK), EXCEEDING_THE_MAXIMUM_PACKET_SIZE_ERROR);

    reinterpret_cast<AccessFieldsClass *>(&receiver)->_fsm_state =
        AccessFieldsClass::receiver_fsm_state_t::PACKET_SIZE_A;
    receiver.set_max_packet_size(200);
    EXPECT_EQ(receiver.parse_packet_byte(255, STREAM_READ_OK), EXCEEDING_THE_MAXIMUM_PACKET_SIZE_ERROR);

    reinterpret_cast<AccessFieldsClass *>(&receiver)->_fsm_state =
        AccessFieldsClass::receiver_fsm_state_t::PACKET_SIZE_A;
    reinterpret_cast<AccessFieldsClass *>(&receiver)->_malloc = [](size_t size) -> void * { return nullptr; };
    EXPECT_EQ(receiver.parse_packet_byte(10, STREAM_READ_OK), NOT_ENOUGH_RAM_FOR_PACKET_ERROR);

    reinterpret_cast<AccessFieldsClass *>(&receiver)->_fsm_state = AccessFieldsClass::receiver_fsm_state_t::WAIT_END;
    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_OK), STREAM_READING_ERROR);

    reinterpret_cast<AccessFieldsClass *>(&receiver)->_fsm_state = AccessFieldsClass::receiver_fsm_state_t::WAIT_END;
    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_DELIMITER), PACKET_HANDLER_NOT_SET_ERROR);

    reinterpret_cast<AccessFieldsClass *>(&receiver)->_fsm_state = static_cast<AccessFieldsClass::receiver_fsm_state_t>(
        static_cast<unsigned int>(AccessFieldsClass::receiver_fsm_state_t::WAIT_END) + 1);
    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_OK), UNKNOWN_READER_ERROR);
}

TEST(bdsp_v1_receiver_tests, checksum_error_test) {
    class ChecksumTester: public BDSP::core::BDSPV1ChecksumMixin {
    public:
        uint8_t calc(BDSP::core::packet_v1_header header, uint8_t *data_ptr, uint16_t size) {
            return _calc_checksum(header, data_ptr, size);
        }
    };

    BDSPReceiver receiver;
    ChecksumTester checksum_tester;

    receiver.set_checksum_function(checksums::crc8_nrsc5);
    checksum_tester.set_checksum_function(checksums::crc8_nrsc5);

    BDSP::core::packet_v1_header header = {.packet_id = 0,
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

TEST(bdsp_v1_receiver_tests, normal_using_test) {
    VectorTestReader reader;
    BDSPReceiver receiver;
    EXPECT_EQ(receiver.set_stream_reader(&reader), SET_STREAM_READER_OK);

    auto read_strategy = reader.get_strategy();

    receiver.set_error_handler(
        [](parse_packet_status_t error, void *ctx) {
            std::cout << "Error: " << error << std::endl;
            FAIL();
        },
        nullptr);

    bool got_packet = false;

    receiver.set_packet_handler(
        [](packet_context_t &packet_context, void *ctx) {
            EXPECT_EQ(packet_context.packet_id, 0);
            EXPECT_EQ(packet_context.size, 1);
            EXPECT_EQ(packet_context.data_ptr[0], 99);
            *reinterpret_cast<bool *>(ctx) = true;
        },
        &got_packet);

    BDSP::core::packet_v1_header header = {.packet_id = 0,
                                           .is_two_bytes_for_packet_size = false,
                                           .is_checksum_used = false,
                                           .is_unsupported_protocol_version = false};

    std::vector<uint8_t> data = {99};
    EXPECT_EQ(read_strategy.read(*reinterpret_cast<uint8_t *>(&header)), STREAM_READ_OK);
    EXPECT_EQ(reader.read(data.size()), STREAM_READ_OK); // data size
    for (auto byte: data) {
        EXPECT_EQ(reader.read(byte), STREAM_READ_OK); // data
    }
    EXPECT_EQ(read_strategy.read_with_status(0xFF, STRATEGY_READ_DELIMITER), STREAM_READ_DELIMITER);
    ASSERT_TRUE(got_packet);
}

TEST(bdsp_v1_receiver_tests, normal_using_with_big_packet_and_no_clear_test) {
    VectorTestReader reader;
    BDSPReceiver receiver;
    EXPECT_EQ(receiver.set_stream_reader(&reader), SET_STREAM_READER_OK);

    receiver.set_error_handler(
        [](parse_packet_status_t error, void *ctx) {
            std::cout << "Error: " << error << std::endl;
            FAIL();
        },
        nullptr);

    uint8_t *packet_data_ptr = nullptr;

    receiver.set_packet_handler(
        [](packet_context_t &packet_context, void *ctx) {
            EXPECT_EQ(packet_context.packet_id, 0);
            EXPECT_EQ(packet_context.size, 300);
            for (int i = 0; i < packet_context.size; ++i) {
                EXPECT_EQ(packet_context.data_ptr[i], 99);
            }
            packet_context.need_clear = false;
            *reinterpret_cast<uint8_t **>(ctx) = packet_context.data_ptr;
        },
        &packet_data_ptr);

    BDSP::core::packet_v1_header header = {.packet_id = 0,
                                           .is_two_bytes_for_packet_size = false,
                                           .is_checksum_used = false,
                                           .is_unsupported_protocol_version = false};

    std::vector<uint8_t> data;
    data.reserve(300);
    for (int i = 0; i < 300; ++i) {
        data.push_back(99);
    }
    header.is_two_bytes_for_packet_size = data.size() > 255;

    EXPECT_EQ(reader.read(*reinterpret_cast<uint8_t *>(&header)), STREAM_READ_OK);
    EXPECT_EQ(reader.read(data.size() >> 8), STREAM_READ_OK); // data size
    EXPECT_EQ(reader.read(data.size()), STREAM_READ_OK);      // data size
    for (auto byte: data) {
        EXPECT_EQ(reader.read(byte), STREAM_READ_OK); // data
    }
    EXPECT_EQ(reader.get_strategy().read_with_status(0x00, STRATEGY_READ_DELIMITER), STREAM_READ_DELIMITER);
    EXPECT_NE(packet_data_ptr, nullptr);
    for (int i = 0; i < data.size(); ++i) {
        ASSERT_TRUE(packet_data_ptr[i] == data[i]);
    }
    free(packet_data_ptr);
}

TEST(bdsp_v1_receiver_tests, correct_checksum_test) {
    class ChecksumTester: public BDSP::core::BDSPV1ChecksumMixin {
    public:
        using BDSP::core::BDSPV1ChecksumMixin::_calc_checksum;
    };

    ChecksumTester tester;
    BDSPReceiver receiver;
    receiver.set_checksum_function(checksums::crc8_nrsc5);
    tester.set_checksum_function(checksums::crc8_nrsc5);

    receiver.set_error_handler(
        [](parse_packet_status_t error, void *ctx) {
            std::cout << "Error: " << error << std::endl;
            FAIL();
        },
        nullptr);

    receiver.set_packet_handler(
        [](packet_context_t &packet_context, void *ctx) {
            EXPECT_EQ(packet_context.packet_id, 0);
            EXPECT_EQ(packet_context.size, 1);
            EXPECT_EQ(packet_context.data_ptr[0], 99);
        },
        nullptr);

    BDSP::core::packet_v1_header header = {.packet_id = 0,
                                           .is_two_bytes_for_packet_size = false,
                                           .is_checksum_used = true,
                                           .is_unsupported_protocol_version = false};

    std::vector<uint8_t> data = {99};
    EXPECT_EQ(receiver.parse_packet_byte(*reinterpret_cast<uint8_t *>(&header), STREAM_READ_OK), STREAM_READ_OK);
    EXPECT_EQ(receiver.parse_packet_byte(data.size(), STREAM_READ_OK), STREAM_READ_OK); // data size
    for (auto byte: data) {
        EXPECT_EQ(receiver.parse_packet_byte(byte, STREAM_READ_OK), STREAM_READ_OK); // data
    }
    EXPECT_EQ(receiver.parse_packet_byte(tester._calc_checksum(header, data.data(), data.size()), STREAM_READ_OK),
              STREAM_READ_OK);
    EXPECT_EQ(receiver.parse_packet_byte(0x00, STREAM_READ_DELIMITER), PARSE_PACKET_BYTE_OK);
}