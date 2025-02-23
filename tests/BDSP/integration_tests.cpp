#include <gtest/gtest.h>

#include "BDSP/transceiver.h"
#include "BDSP/streams/cobs/reader.h"
#include "BDSP/streams/cobs/writer.h"

using namespace BDSP;
using namespace BDSP::streams;
using namespace BDSP::streams::cobs;

TEST(bdsp_full_tests, bdsp_v1_full_test) {
    COBSReaderStream reader;
    COBSWriterStream writer;
    bool is_packet_got = false;

    writer.set_stream_writer([] (uint8_t byte, void *ctx) {
        reinterpret_cast<COBSReaderStream*>(ctx)->read(byte);
    }, &reader);

    BDSPTransceiver transceiver1;
    transceiver1.set_stream_writer(&writer);
    transceiver1.set_stream_reader(&reader);

    transceiver1.set_packet_handler([] (packet_context_t &packet_context, void *ctx){
        *reinterpret_cast<bool*>(ctx) = true;
    }, &is_packet_got);

    transceiver1.set_error_handler([] (parse_packet_status_t error, void *ctx){
        std::cout << "Error: " << error << std::endl;
        FAIL();
    }, nullptr);


    uint16_t size = 65535;
    auto *data = new uint8_t[size];

    for (int i = 0; i < size; ++i) {
        data[i] = 0xAA;
    }

    transceiver1.send_data(0, data, size);

    delete [] data;

    ASSERT_TRUE(is_packet_got);
}