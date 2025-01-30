#include <gtest/gtest.h>

#include "BDSP/streams/COBS/reader.h"
#include "BDSP/streams/COBS/writer.h"
#include "BDSP/transceiver.h"

#include "../utils/testing.h"

using namespace BDSP;
using namespace BDSP::streams;

TEST(bdsp_tests, bdsp_full_test) {
    struct Context {
        std::vector<uint8_t> data;
        bdsp_packet_context_t *packet_context = nullptr;
        bool is_packet_got = false;
        BDSPTransceiver transceiver;
        COBS::COBSWriter writer;
        COBS::COBSReader reader;
    } ctx;

    ctx.transceiver.set_writer(&ctx.writer);
    ctx.transceiver.set_stream_reader(&ctx.reader);

    ctx.writer.set_stream_writer([](uint8_t byte, void *ctx) {
        reinterpret_cast<Context *>(ctx)->transceiver.parse(byte);
    }, &ctx);

    ctx.transceiver.set_packet_handler([] (bdsp_packet_context_t &packet_context, void *ctx) {
        auto *context = reinterpret_cast<Context*>(ctx);
        auto correct_data = context->data;
        bool equals = is_equals(packet_context.data_ptr, packet_context.size, correct_data);
        if (equals) {
            context->is_packet_got = true;
        }
        ASSERT_TRUE(equals);

    }, &ctx);


    for (int i = 0; i < 500; ++i) ctx.data.push_back(i);
    bdsp_packet_context_t packet_context = {0, ctx.data.data(), static_cast<uint16_t>(ctx.data.size())};
    ctx.packet_context = &packet_context;

    ctx.transceiver.send_packet(packet_context, true);
    ASSERT_TRUE(ctx.is_packet_got);
}
