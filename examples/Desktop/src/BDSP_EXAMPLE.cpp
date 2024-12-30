#include <iostream>
#include <vector>

#include <BDSP/transceiver.h>
#include <BDSP/encoders/COBS.h>
#include <BDSP/decoders/COBS.h>

#include "utils.h"

using namespace BDSP;

using namespace encoders::COBS;
using namespace decoders::COBS;


int main() {
    std::vector<uint8_t> data;
    for (int i = 0; i < 10; ++i) data.push_back(i);
    Packet packet(0, data.size(), data.data());

    BDSPTransceiver transceiver;

    COBSEncoder encoder;
    transceiver.set_encoder(&encoder);

    COBSDecoder decoder;
    transceiver.set_decoder(&decoder);

    encoder.set_writer([] (uint8_t byte, void *ctx) {
        // This lambda function implements the sending of encoded data to the communication channel.
        // But for the current example, they are sent directly to the BDSPTransceiver, forming pipeline.
        //
        // In real-world use, you need to write a callback function that takes data from a communication channel and
        // passes it to a BDSPTransceiver or BDSPReceiver or configured decoder.
        reinterpret_cast<BDSPTransceiver*>(ctx)->parse(byte);
    }, &transceiver);

    transceiver.set_packet_handler([] (Packet &packet, void *ctx) {
        std::cout << "Got packet! Id: " << static_cast<uint32_t>(packet.id) << std::endl << "Packet ";
        show_data(packet.data_ptr, packet.size);
    }, nullptr);

    transceiver.send_packet(packet);

    return 0;
}
