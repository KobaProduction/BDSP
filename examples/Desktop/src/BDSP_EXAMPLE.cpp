#include <iostream>
#include <vector>

#include <BDSP/streams/COBS/reader.h>
#include <BDSP/streams/COBS/writer.h>
#include <BDSP/transceiver.h>

#include "utils.h"

using namespace BDSP;
using namespace BDSP::streams::COBS;

int main() {
    std::vector<uint8_t> data;
    for (int i = 1; i < 10; ++i)
        data.push_back(i);
    bdsp_packet_context_t packet_context;

    packet_context.packet_id = 0;
    packet_context.size = data.size();
    packet_context.data_ptr = data.data();

    BDSPTransceiver transceiver;

    COBSWriter writer;
    transceiver.set_stream_writer(&writer);

    COBSReader reader;
    transceiver.set_stream_reader(&reader);

    writer.set_stream_writer(
        [](uint8_t byte, void *ctx) {
            reinterpret_cast<streams::IReader *>(ctx)->read(byte);
        },
        &reader);

    transceiver.set_packet_handler(
        [](bdsp_packet_context_t &packet_ctx, void *ctx) {
            std::cout << "Got packet! Id: " << static_cast<uint32_t>(packet_ctx.packet_id);
            std::cout << ", size: " << uint32_t(packet_ctx.size) << std::endl;
            std::cout << "Packet ";
            show_data_(packet_ctx.data_ptr, packet_ctx.size);
        },
        nullptr);

    transceiver.set_error_handler(
        [](bdsp_receiver_parse_status_t status, void *ctx) {
            std::cout << "bdsp_receiver_parse_error_status_t: " << static_cast<uint32_t>(status) << std::endl;
        },
        nullptr);

    transceiver.send_packet(packet_context);

    return 0;
}
