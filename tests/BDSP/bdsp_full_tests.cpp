#include <gtest/gtest.h>

#include "BDSP/transceiver.h"
#include "BDSP/streams/COBS/reader.h"
#include "BDSP/streams/COBS/writer.h"

using namespace BDSP;
using namespace BDSP::streams;
using namespace BDSP::streams::COBS;

TEST(bdsp_full_tests, bdsp_v1_full_test) {
    COBSReader reader;
    COBSWriter writer;

    writer.set_stream_writer([] (uint8_t byte, void *ctx) {
        reinterpret_cast<COBSReader*>(ctx)->read(byte);
    }, &reader);

    BDSPTransceiver transceiver1;
    transceiver1.set_stream_writer(&writer);
    transceiver1.set_stream_reader(&reader);

    transceiver1.set_packet_handler([] (packet_context_t &packet_context, void *ctx){
        std::cout << "Got packet: " << packet_context.packet_id << std::endl;
    }, nullptr);

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
}