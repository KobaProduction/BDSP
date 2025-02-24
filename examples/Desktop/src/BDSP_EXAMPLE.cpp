#include <iostream>
#include <vector>

#include <BDSP/streams/cobs/reader.h>
#include <BDSP/streams/cobs/writer.h>
#include <BDSP/transceiver.h>

#include "utils.h"

using namespace BDSP;
using namespace BDSP::streams::cobs;

int main() {


    BDSPTransceiver transceiver;

    COBSZPEWriterStream writer;
    COBSZPEReaderStream reader;

    writer.set_stream_writer(
        [](uint8_t byte, void *ctx) {
            reinterpret_cast<COBSZPEReaderStream *>(ctx)->read(byte);
        },
        &reader);


    transceiver.set_stream_writer(&writer);
    transceiver.set_stream_reader(&reader);
    transceiver.set_packet_handler(
        [](packet_context_t &packet_ctx, void *ctx) {
            std::cout << "Got packet! Id: " << static_cast<uint32_t>(packet_ctx.packet_id);
            std::cout << ", size: " << uint32_t(packet_ctx.size) << std::endl;
            std::cout << "Packet ";
            show_data_(packet_ctx.data_ptr, packet_ctx.size);
        },
        nullptr);

    transceiver.set_error_handler(
        [](parse_packet_status_t status, void *ctx) {
            std::cout << "parse_packet_status_t error: " << static_cast<uint32_t>(status) << std::endl;
        },
        nullptr);

    std::vector<uint8_t> data;
    for (int i = 1; i < 10; ++i) {
        data.push_back(i);
    }

    packet_context_t packet_context;
    packet_context.packet_id = 0;
    packet_context.size = data.size();
    packet_context.data_ptr = data.data();

    transceiver.send_packet(packet_context);
    return 0;
}
