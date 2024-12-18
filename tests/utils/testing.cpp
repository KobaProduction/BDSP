#include <gtest/gtest.h>

#include "testing.h"
#include "show.h"

bool is_equals(const uint8_t *data1, size_t size1, const uint8_t *data2, size_t size2) {
    if (size1 not_eq size2) return false;
    bool equals = true;
    for (size_t i = 0; i < size1; ++i) {
        if (data1[i] == data2[i]) continue;
        equals = false;
        break;
    }
    return equals;
}

bool is_equals(const uint8_t *data1, size_t size1, std::vector<uint8_t> data2) {
    return is_equals(data1, size1, data2.data(), data2.size());
}

bool is_equals(std::vector<uint8_t> data1, std::vector<uint8_t> data2) {
    return is_equals(data1.data(), data1.size(), data2.data(), data2.size());
}

void start_test_encoder(
        BDSP::encoders::ABS::AbstractEncoder &encoder,
        std::vector<uint8_t> &data,
        std::vector<uint8_t> &correct_encoded,
        show_t type
) {
    std::vector<uint8_t> encoded_buffer;

    encoder.set_writer([](uint8_t byte, void *ctx_ptr) {
        reinterpret_cast<std::vector<uint8_t> *>(ctx_ptr)->push_back(byte);
    }, &encoded_buffer);

    auto status = encoder.encode(data.data(), data.size());
    encoder.finish_encode();
    if (status not_eq BDSP::encoders::ENCODE_OK) {
        show_state(status);
        FAIL() << "Incorrect state";
    }
    if (is_equals(correct_encoded, encoded_buffer)) return;

    std::cout << "Correct ";
    show_data(correct_encoded, type);
    std::cout << "Encoded ";
    show_data(encoded_buffer, type);
    FAIL() << "the correct and encoded array is not equal";
}

void start_test_decoder(
        BDSP::decoders::ABS::AbstractDecoder &decoder,
        std::vector<uint8_t> &encoded,
        std::vector<uint8_t> &correct_decoded,
        show_t type
) {
    struct Context {
        std::vector<uint8_t> decoded;
        bool is_ended = false;
    } ctx;

    decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx_ptr) {
        auto context = reinterpret_cast<Context *>(ctx_ptr);
        ASSERT_FALSE(context->is_ended);
        switch (status) {
            case DECODE_OK:
                context->decoded.push_back(byte);
                break;
            case DECODE_END:
                context->is_ended = true;
                break;
            default:
                std::cout << "Symbol: " << uint32_t(byte) << " - ";
                show_state(status);
                FAIL() << "Incorrect state";
        }

    }, &ctx);

    decoder.decode(encoded.data(), encoded.size());
    if (is_equals(correct_decoded, ctx.decoded)) return;
    std::cout << "Correct ";
    show_data(correct_decoded, type);
    std::cout << "Decoded ";
    show_data(ctx.decoded, type);
    FAIL() << "the correct and decoded array is not equal";
}
