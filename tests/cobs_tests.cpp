#include <gtest/gtest.h>

#include <string>
#include "utils/show.h"
#include "utils/cobs.h"
#include "BDSP/encoders/COBS.h"

using namespace BDSP::encoders::COBS;

struct Context {
    std::vector<uint8_t> encoded_buffer;
    uint8_t *cobs_buffer = nullptr;
    size_t cobs_wrote = 0;
    COBSEncoder cobs_encoder = COBSEncoder();
};

Context context;

void start_test(uint8_t *data, size_t size, Context &ctx) {
    if (not size) return;
    ctx.encoded_buffer.clear();
    free(ctx.cobs_buffer);
    size_t cobs_buffer_size = 10 + size + size / 10;
    ctx.cobs_buffer = static_cast<uint8_t *>(malloc(cobs_buffer_size));
    for (int i = 0; i < cobs_buffer_size; ++i) ctx.cobs_buffer[i] = 0;
    ctx.cobs_wrote = 0;

    auto status = ctx.cobs_encoder.encode(data, size);
    ctx.cobs_encoder.finish_encode();
    if (status not_eq BDSP::encoders::ENCODE_OK) {
        show_state(status);
        EXPECT_EQ(status, BDSP::encoders::ENCODE_OK);
        return;
    }

    StuffData(data, size, ctx.cobs_buffer);
    while (ctx.cobs_buffer[ctx.cobs_wrote++] != 0x00);

    bool equals = ctx.encoded_buffer.size() == ctx.cobs_wrote;

    if (equals) {
        for (int i = 0; i < ctx.encoded_buffer.size(); ++i) {
            if (ctx.encoded_buffer[i] == ctx.cobs_buffer[i]) continue;
            equals = false;
            break;
        }
    }
    if (equals) return;

    std::cout << "COBS     ";
    show_data(ctx.cobs_buffer, ctx.cobs_wrote);
    std::cout << "Encoded  ";
    show_data(ctx.encoded_buffer.data(), ctx.encoded_buffer.size(), DEC);
    EXPECT_EQ(status, true);
}

TEST(cobs_encoding_test, cobs_encoding_test_1_1) {
    context.cobs_encoder.set_writer([](uint8_t byte, void *ctx_ptr) {
        auto &context = *reinterpret_cast<Context *>(ctx_ptr);
        context.encoded_buffer.push_back(byte);
    }, &context);

    uint8_t *data = nullptr;
    for (int size = 1; size < 1000; ++size) {
        free(data);
        data = static_cast<uint8_t *>(malloc(size));
        for (int i = 0; i < size; ++i) data[i] = i;
        start_test(data, size, context);
    }

    for (int size = 1; size < 1000; ++size) {
        free(data);
        data = static_cast<uint8_t *>(malloc(size));
        for (int i = 0; i < size; ++i) data[i] = 0x00;
        start_test(data, size, context);
    }
    for (int size = 1; size < 1000; ++size) {
        free(data);
        data = static_cast<uint8_t *>(malloc(size));
        for (int i = 0; i < size; ++i) data[i] = 0xFF;
        start_test(data, size, context);
    }

    free(data);
    free(context.cobs_buffer);
}
