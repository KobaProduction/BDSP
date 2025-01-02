#include <iostream>
#include <vector>

#include <BDSP/transceiver.h>
#include <BDSP/streams/COBS/writer.h>
#include <BDSP/streams/COBS/reader.h>

#include "utils.h"

using namespace BDSP;
using namespace BDSP::streams;


int main() {
    std::vector<uint8_t> data;
    for (int i = 0; i < 10; ++i) data.push_back(i);
    Packet packet(0, data.size(), data.data());

    BDSPTransceiver transceiver;

    COBS::COBSWriter writer;
    transceiver.set_writer(&writer);

    COBS::COBSReader reader;
    transceiver.set_reader(&reader);

    writer.set_stream_writer([](uint8_t byte, void *ctx) {
        // This lambda function implements the sending of encoded data to the communication channel.
        // But for the current example, they are sent directly to the BDSPTransceiver, forming pipeline.
        //
        // In real-world use, you need to read a callback function that takes data from a communication channel and
        // passes it to a BDSPTransceiver or BDSPReceiver or configured decoder.
        reinterpret_cast<BDSPTransceiver *>(ctx)->parse(byte);
    }, &transceiver);

    transceiver.set_packet_handler([] (Packet &packet, void *ctx) {
        std::cout << "Got packet! Id: " << static_cast<uint32_t>(packet.id) << std::endl << "Packet ";
        show_data_(packet.data_ptr, packet.size);
    }, nullptr);

    transceiver.send_packet(packet);

    return 0;
}
