#include <iostream>
#include <vector>

#include "BDSP/decoders/PPP.h"
#include "BDSP/encoders/PPP.h"
#include "../../tests/utils/show.h"

using namespace BDSP::decoders::PPP;
using namespace BDSP::encoders::PPP;

struct Context {
    std::vector<uint8_t> encoded_buffer;
    std::vector<uint8_t> decoded_buffer;
};

int main() {
    Context context;

    size_t size = 10;
    uint8_t data[size];
    for (uint8_t &i: data) i = 1;
    data[5] = 0x7D;
    data[6] = 0x7E;

    std::cout << "Data (" << size << ") ";
    show_data(data, size, HEX);

    auto ppp_encoder = PPPEncoder();
    ppp_encoder.set_writer([](uint8_t byte, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "encode writer ctx_ptr is null!" << std::endl;
            return;
        };
        auto &context = *reinterpret_cast<Context *>(ctx_ptr);
        context.encoded_buffer.push_back(byte);
    }, &context);
    auto encode_status = ppp_encoder.encode(data, size);
    if (encode_status != ENCODE_OK) {
        std::cout << "Encode Status: ";
        show_status(encode_status);
    }

    if (context.encoded_buffer.empty()) return 0;
    std::cout << "Encoded Buffer (" << context.encoded_buffer.size() << ") ";
    show_data(context.encoded_buffer.data(), context.encoded_buffer.size(), HEX);

    auto ppp_decoder = PPPDecoder();
    ppp_decoder.set_data_handler([](uint8_t byte, decode_status_t state, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "decode data handler ctx_ptr is null!" << std::endl;
            return;
        };
        auto &context = *reinterpret_cast<Context *>(ctx_ptr);
        context.decoded_buffer.push_back(byte);
    }, &context);

    auto decode_status = ppp_decoder.decode(context.encoded_buffer.data(), context.encoded_buffer.size());
    if (decode_status not_eq DECODE_OK) {
        std::cout << "Decode Status: ";
        show_status(encode_status);
    }

    if (context.decoded_buffer.empty()) return 0;
    std::cout << "Decoded Buffer (" << context.decoded_buffer.size() << ") ";
    show_data(context.decoded_buffer.data(), context.decoded_buffer.size(), HEX);
    return 0;
}
