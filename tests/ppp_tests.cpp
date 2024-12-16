#include <gtest/gtest.h>

#include "utils/show.h"
#include "BDSP/encoders/PPP.h"
#include "BDSP/decoders/PPP.h"

using namespace BDSP::encoders::PPP;
using namespace BDSP::decoders::PPP;

struct ContextPPP {
    std::vector<uint8_t> encoded_buffer;
    std::vector<uint8_t> decoded_buffer;
    PPPEncoder encoder;
    PPPDecoder decoder;
};

TEST(ppp_encoding_test, encoding_and_decoding_test) {
    ContextPPP context;

    context.encoder.set_writer([](uint8_t byte, void *ctx_ptr) {
        auto &context = *reinterpret_cast<ContextPPP *>(ctx_ptr);
        context.encoded_buffer.push_back(byte);
    }, &context);

    context.decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *context) {
        if (status not_eq DECODE_OK) return;
        reinterpret_cast<ContextPPP *>(context)->decoded_buffer.push_back(byte);
    }, &context);

    uint8_t data[3] = {0x7E, 0x7D, 0x00};
    context.encoder.encode(data, 3);
    context.encoder.finish_encode();

    context.decoder.decode(context.encoded_buffer.data(), context.encoded_buffer.size());

    bool equals = 3 == context.decoded_buffer.size();

    if (equals) {
        for (int i = 0; i < 3; ++i) {
            if (data[i] == context.decoded_buffer[i]) continue;
            equals = false;
            break;
        }
    }
    if (equals) return;
    std::cout << "Data        ";
    show_data(data, 3);
    std::cout << "PPP encoded ";
    show_data(context.encoded_buffer.data(), context.encoded_buffer.size());
    std::cout << "PPP decoded ";
    show_data(context.decoded_buffer.data(), context.decoded_buffer.size(), DEC);
    EXPECT_EQ(equals, true);
}

TEST(ppp_encoding_test, decoding_error_test) {
    ContextPPP context;

    context.encoder.set_writer([](uint8_t byte, void *ctx_ptr) {
        auto &context = *reinterpret_cast<ContextPPP *>(ctx_ptr);
        context.encoded_buffer.push_back(byte);
    }, &context);

    context.decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *context) {
        if (status not_eq DECODE_OK) return;
        reinterpret_cast<ContextPPP *>(context)->decoded_buffer.push_back(byte);
    }, &context);

    uint8_t data[3] = {0x7E, 0x7D, 0x00};
    context.encoder.encode(data, 3);
    context.encoder.finish_encode();
    context.encoded_buffer[1] = 0x7D;
    auto status = context.decoder.decode(context.encoded_buffer.data(), context.encoded_buffer.size());
    EXPECT_EQ(status, DECODE_ERROR);
}
