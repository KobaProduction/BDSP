#include <gtest/gtest.h>

#include "BDSP/transmitters.h"
#include "BDSP/transceivers.h"
#include "BDSP/receivers.h"

#include "testing.h"

using namespace BDSP;
using namespace BDSP::streams;

TEST(bdsp_full_tests, transceiver_test) {
    bool is_packet_got = false;
    COBSTransceiver transceiver;
    transceiver.get_write_stream().set_stream_writer(
        [](uint8_t byte, void *ctx) { reinterpret_cast<IStreamReader *>(ctx)->read(byte); },
        &transceiver.get_read_stream());
    transceiver.set_packet_handler(
        [](packet_context_t &packet_context, void *ctx) { *reinterpret_cast<bool *>(ctx) = true; }, &is_packet_got);
    transceiver.set_error_handler(
        [](parse_packet_status_t error, void *ctx) {
            std::cout << "Error: " << error << std::endl;
            FAIL();
        },
        nullptr);
    uint16_t size = 65535;
    std::vector<uint8_t> data;
    data.resize(size, 0xAA);
    transceiver.send_data(0, data.data(), data.size());
    ASSERT_TRUE(is_packet_got);
}

TEST(bdsp_full_tests, transmitter_test) {
    bool is_got_bytes = false;
    COBSTransmitter transmitter;
    std::vector<uint8_t> raw_data;
    transmitter.get_write_stream().set_stream_writer(
        [](uint8_t byte, void *ctx) { reinterpret_cast<std::vector<uint8_t> *>(ctx)->push_back(byte); }, &raw_data);
    transmitter.send_data(0, reinterpret_cast<uint8_t *>(&is_got_bytes), 1);
    ASSERT_TRUE(is_equals(raw_data, {3, 64, 1, 2, 77, 0}));
}

TEST(bdsp_full_tests, receiver_test) {
    bool is_packet_got = false;
    COBSReceiver receiver;
    receiver.set_packet_handler(
        [](packet_context_t &packet_context, void *ctx) { *reinterpret_cast<bool *>(ctx) = true; }, &is_packet_got);
    receiver.set_error_handler(
        [](parse_packet_status_t error, void *ctx) {
            std::cout << "Error: " << error << std::endl;
            FAIL();
        },
        nullptr);
    std::vector<uint8_t> raw_data = {3, 64, 1, 2, 77, 0};
    receiver.get_read_stream().read(raw_data.data(), raw_data.size());
    ASSERT_TRUE(is_packet_got);
}
