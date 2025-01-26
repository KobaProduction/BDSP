#include <iostream>
#include <vector>

#include <BDSP/transceiver.h>
#include <BDSP/streams/COBS/writer.h>
#include <BDSP/streams/COBS/reader.h>

#include "utils.h"

using namespace BDSP;
using namespace BDSP::streams::COBS;


int main() {
    std::vector<uint8_t> data;
    for (int i = 1; i < 10; ++i) data.push_back(i);
    bdsp_packet_t packet;

//    packet.id = 0;
    packet.size = data.size();
    packet.data_ptr = data.data();

    BDSPTransceiver transceiver;

    COBSWriter writer;
    transceiver.set_writer(&writer);

    COBSReader reader;
    transceiver.set_reader(&reader);

    writer.set_stream_writer([](uint8_t byte, void *ctx) {
        // This lambda function implements the sending of encoded data to the communication channel.
        // But for the current example, they are sent directly to the BDSPTransceiver, forming pipeline.
        //
        // In real-world use, you need to read a callback function that takes data from a communication channel and
        // passes it to a BDSPTransceiver or BDSPReceiver or configured decoder.
//        std::cout << uint32_t(byte) << " ";
        reinterpret_cast<BDSPTransceiver *>(ctx)->parse(byte);
    }, &transceiver);

    transceiver.set_packet_handler([] (bdsp_packet_context_t &packet_ctx, void *ctx) {
        std::cout << "Got packet! Id: " << static_cast<uint32_t>(packet_ctx.packet_id);
        std:: cout << ", size: " << uint32_t(packet_ctx.packet.size) << std::endl;
        std:: cout << "Packet ";
        show_data_(packet_ctx.packet.data_ptr, packet_ctx.packet.size);
    }, nullptr);

    transceiver.send_packet(1, packet);

    return 0;
}
