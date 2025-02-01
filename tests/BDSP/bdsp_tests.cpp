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
        packet_context_t *packet_context = nullptr;
        bool is_packet_got = false;
        BDSPTransceiver transceiver;
        COBS::COBSWriter writer;
        COBS::COBSReader reader;
    } ctx;

    ctx.writer.set_stream_writer([](uint8_t byte, void *ctx) {
        reinterpret_cast<Context *>(ctx)->reader.read(byte);
    }, &ctx);

    ctx.transceiver.set_stream_writer(&ctx.writer);
    EXPECT_EQ(ctx.transceiver.set_stream_reader(&ctx.reader), SET_STREAM_READER_OK);

    ctx.transceiver.set_packet_handler([] (packet_context_t &packet_context, void *ctx) {
        auto *context = reinterpret_cast<Context*>(ctx);
        auto correct_data = context->data;
        bool equals = is_equals(packet_context.data_ptr, packet_context.size, correct_data);
        if (equals) {
            context->is_packet_got = true;
        }
        ASSERT_TRUE(equals);

    }, &ctx);


    for (int i = 0; i < 500; ++i) ctx.data.push_back(i);
    packet_context_t packet_context = {0, ctx.data.data(), static_cast<uint16_t>(ctx.data.size())};
    ctx.packet_context = &packet_context;

    EXPECT_EQ(ctx.transceiver.send_packet(packet_context), SEND_PACKET_OK);
    ASSERT_TRUE(ctx.is_packet_got);
}
