#include <gtest/gtest.h>

#include "utils/show.h"
#include "utils/cobs.h"
#include "BDSP/encoders/COBS.h"
#include "BDSP/decoders/COBS.h"

using namespace BDSP::encoders::COBS;
using namespace BDSP::decoders::COBS;

struct ContextCOBS {
    std::vector<uint8_t> encoded_buffer;
    std::vector<uint8_t> decoded_buffer;
    uint8_t *cobs_buffer = nullptr;
    size_t cobs_wrote = 0;
    COBSEncoder cobs_encoder = COBSEncoder();
    COBSDecoder cobs_decoder = COBSDecoder();
};

void start_test_encoder(uint8_t *data, size_t size, ContextCOBS &ctx) {
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


TEST(cobs_encoding_test, encoding_test) {
    ContextCOBS context;
    context.cobs_encoder.set_writer([](uint8_t byte, void *ctx_ptr) {
        auto &context = *reinterpret_cast<ContextCOBS *>(ctx_ptr);
        context.encoded_buffer.push_back(byte);
    }, &context);

    uint8_t *data = nullptr;
    for (int size = 1; size < 1000; ++size) {
        free(data);
        data = static_cast<uint8_t *>(malloc(size));

        for (int i = 0; i < size; ++i) data[i] = i;
        start_test_encoder(data, size, context);

        for (int i = 0; i < size; ++i) data[i] = 0x00;
        start_test_encoder(data, size, context);

        for (int i = 0; i < size; ++i) data[i] = 0xFF;
        start_test_encoder(data, size, context);
    }

    free(data);
    free(context.cobs_buffer);
}

void start_test_decoder(uint8_t *data, size_t size, ContextCOBS &ctx) {
    if (not size) return;
    ctx.encoded_buffer.clear();
    ctx.decoded_buffer.clear();

    auto status = ctx.cobs_encoder.encode(data, size);
    ctx.cobs_encoder.finish_encode();
    if (status not_eq BDSP::encoders::ENCODE_OK) {
        show_state(status);
        EXPECT_EQ(status, BDSP::encoders::ENCODE_OK);
        return;
    }

    ctx.cobs_decoder.decode(ctx.encoded_buffer.data(), ctx.encoded_buffer.size());
//    StuffData(data, size, ctx.cobs_buffer);

    bool equals = size == ctx.decoded_buffer.size();

    if (equals) {
        for (int i = 0; i < size; ++i) {
            if (data[i] == ctx.decoded_buffer[i]) continue;
            equals = false;
            break;
        }
    }
    if (equals) return;
    std::cout << "Data     ";
    show_data(data, size);
    std::cout << "COBS     ";
    show_data(ctx.encoded_buffer.data(), ctx.encoded_buffer.size());
    std::cout << "Decoded  ";
    show_data(ctx.decoded_buffer.data(), ctx.decoded_buffer.size(), DEC);
    EXPECT_EQ(status, true);
}

TEST(cobs_encoding_test, decoding_test) {
    ContextCOBS context;
    context.cobs_encoder.set_writer([](uint8_t byte, void *ctx_ptr) {
        auto &context = *reinterpret_cast<ContextCOBS *>(ctx_ptr);
        context.encoded_buffer.push_back(byte);
    }, &context);

    context.cobs_decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "decode data handler ctx_ptr is null!" << std::endl;
            return;
        }
        switch (status) {
            case DECODE_OK:
                reinterpret_cast<ContextCOBS *>(ctx_ptr)->decoded_buffer.push_back(byte);
                break;
            case DECODE_END:
//                std::cout << "end" << std::endl;
                break;
            case DECODE_ERROR:
            case DATA_HANDLER_NOT_INSTALLED:
                std::cout << "Symbol: " << uint32_t(byte) << " - ";
                show_state(status);
                EXPECT_EQ(false, true);
                break;
        }
    }, &context);

    uint8_t *data = nullptr;
    for (int size = 1; size < 1000; ++size) {
        free(data);
        data = static_cast<uint8_t *>(malloc(size));

        for (int i = 0; i < size; ++i) data[i] = i;
        start_test_decoder(data, size, context);

        for (int i = 0; i < size; ++i) data[i] = 0x00;
        start_test_decoder(data, size, context);

        for (int i = 0; i < size; ++i) data[i] = 0xFF;
        start_test_decoder(data, size, context);
    }

    free(data);
}

