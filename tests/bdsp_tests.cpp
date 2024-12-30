#include <gtest/gtest.h>

#include <BDSP/transceiver.h>
#include <BDSP/encoders/COBS.h>
#include <BDSP/decoders/COBS.h>

#include "utils/testing.h"

using namespace BDSP;

using namespace encoders::COBS;
using namespace decoders::COBS;

TEST(bdsp_tests, bdsp_full_test) {
    struct Context {
        std::vector<uint8_t> data;
        Packet *packet_ptr = nullptr;

        BDSPTransceiver transceiver;
        COBSEncoder encoder;
        COBSDecoder decoder;
    } ctx;

    ctx.transceiver.set_encoder(&ctx.encoder);
    ctx.transceiver.set_decoder(&ctx.decoder);

    ctx.encoder.set_writer([] (uint8_t byte, void *ctx) {
        reinterpret_cast<Context*>(ctx)->transceiver.parse(byte);
    }, &ctx);

    ctx.transceiver.set_packet_handler([] (Packet &packet, void *ctx) {
        ASSERT_TRUE(is_equals(packet.data_ptr, packet.size, reinterpret_cast<Context*>(ctx)->data));
    }, &ctx);


    for (int i = 0; i < 10; ++i) ctx.data.push_back(i);
    Packet packet(0, ctx.data.size(), ctx.data.data());
    ctx.packet_ptr = &packet;

    ctx.transceiver.send_packet(packet);
}
