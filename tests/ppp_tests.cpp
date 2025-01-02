#include <cstdint>
#include <vector>
#include <gtest/gtest.h>

#include <BDSP/streams/PPP/writer.h>
#include <BDSP/streams/PPP/reader.h>

#include "utils/testing.h"

using namespace BDSP::streams;


TEST(ppp_pipelines_tests, encoding_test) {
    PPP::PPPWriter writer;

    std::vector<uint8_t> data = {0x7E, 0x7D, 0x00};
    std::vector<uint8_t> correct = {0x7D, 0x5E, 0x7D, 0x5D, 0x00, 0x7E};

    start_test_writer(writer, data, correct, true);
}

TEST(ppp_pipelines_tests, decoding_errors_test) {
    struct Context {
        PPP::PPPDecoder reader;
        std::vector<uint8_t> read_buffer;
        std::vector<uint8_t> write_buffer;
    } ctx;

    ctx.reader.set_data_handler([](uint8_t byte, read_status_t status, void *ctx) {
        if (status == READ_OK) reinterpret_cast<Context *>(ctx)->read_buffer.push_back(byte);
    }, &ctx);

    for (uint8_t byte = 0; byte < 255; ++byte) {
        ctx.reader.reset_read_state(false);
        ctx.read_buffer.clear();
        ctx.write_buffer = {0x7D, byte};
        auto status = ctx.reader.read(ctx.write_buffer.data(), ctx.write_buffer.size());

        if (byte not_eq 0x5D and byte not_eq 0x5E) {
            EXPECT_EQ(ctx.read_buffer.size(), 0);
        } else {
            EXPECT_EQ(status, READ_OK);
            EXPECT_EQ(ctx.read_buffer.size(), 1);
            EXPECT_EQ(ctx.read_buffer[0], byte ^ 0x20);
        }
    }

}

TEST(ppp_pipelines_tests, decoding_normal_test) {
    struct Context {
        PPP::PPPDecoder reader;
        std::vector<uint8_t> write_buffer;
        std::vector<uint8_t> read_buffer;
        std::vector<uint8_t> correct;
    } ctx;

    ctx.write_buffer = {0x00, 0x01, 0x7E};
    ctx.correct = {0x00, 0x01};
    start_test_reader(ctx.reader, ctx.write_buffer, ctx.correct);

    ctx.reader.set_data_handler([](uint8_t byte, read_status_t status, void *ctx) {
        auto &context = *reinterpret_cast<Context *>(ctx);
        switch (status) {
            case READ_OK:
                context.read_buffer.push_back(byte);
                break;
            case READ_END:
                break;
            case READ_ERROR:
            case UNKNOWN_READER_ERROR:
                FAIL() << "An unforeseen mistake arose";
        }
    }, &ctx);

    ctx.write_buffer = {0x7E, 0x7E, 0x00, 0x01, 0x7E};
    ctx.correct = {0x00, 0x01};
    ctx.reader.read(ctx.write_buffer.data(), ctx.write_buffer.size());
    EXPECT_EQ(is_equals(ctx.read_buffer, ctx.correct), true);
}