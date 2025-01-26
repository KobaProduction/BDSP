#include <gtest/gtest.h>

#include <BDSP/transceiver.h>
#include <BDSP/streams/COBS/writer.h>
#include <BDSP/streams/COBS/reader.h>

#include "utils/testing.h"

using namespace BDSP;
using namespace BDSP::streams;

TEST(bdsp_tests, bdsp_full_test) {
    struct Context {
        std::vector<uint8_t> data;
        bdsp_packet_t *packet_ptr = nullptr;

        BDSPTransceiver transceiver;
        COBS::COBSWriter writer;
        COBS::COBSReader reader;
    } ctx;

    ctx.transceiver.set_writer(&ctx.writer);
    ctx.transceiver.set_reader(&ctx.reader);

    ctx.writer.set_stream_writer([](uint8_t byte, void *ctx) {
        reinterpret_cast<Context *>(ctx)->transceiver.parse(byte);
    }, &ctx);

    ctx.transceiver.set_packet_handler([] (bdsp_packet_context_t &packet_context, void *ctx) {
        auto correct_data = reinterpret_cast<Context*>(ctx)->data;
        ASSERT_TRUE(is_equals(packet_context.packet.data_ptr, packet_context.packet.size, correct_data));
    }, &ctx);


    for (int i = 0; i < 10; ++i) ctx.data.push_back(i);
    bdsp_packet_t packet = {static_cast<uint16_t>(ctx.data.size()), ctx.data.data()};
    ctx.packet_ptr = &packet;

    ctx.transceiver.send_packet(0, packet);
}
