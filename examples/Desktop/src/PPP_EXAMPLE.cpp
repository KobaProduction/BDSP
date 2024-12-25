#include <iostream>
#include <vector>

#include <BDSP/decoders/PPP.h>
#include <BDSP/encoders/PPP.h>

#include "utils.h"

using namespace BDSP::decoders::PPP;
using namespace BDSP::encoders::PPP;

struct Context {
    std::vector<uint8_t> encoded_buffer;
    std::vector<uint8_t> decoded_buffer;
};

int main() {
    Context context;

    std::vector<uint8_t> data = {1, 0x7E, 3, 0x7D, 0};
    std::cout << "Default ";
    show_data(data, true);

    auto ppp_encoder = PPPEncoder();
    ppp_encoder.set_writer([](uint8_t byte, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "encode writer ctx_ptr is null!" << std::endl;
            return;
        };
        reinterpret_cast<Context *>(ctx_ptr)->encoded_buffer.push_back(byte);
    }, &context);

    ppp_encoder.encode(data.data(), data.size());

    std::cout << "Encoded ";
    show_data(context.encoded_buffer, true);

    auto ppp_decoder = PPPDecoder();
    ppp_decoder.reset_decode_state(false);
    ppp_decoder.set_data_handler([](uint8_t byte, decode_status_t state, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "decode data handler ctx_ptr is null!" << std::endl;
            return;
        };
        reinterpret_cast<Context *>(ctx_ptr)->decoded_buffer.push_back(byte);
    }, &context);

     ppp_decoder.decode(context.encoded_buffer.data(), context.encoded_buffer.size());

    std::cout << "Decoded ";
    show_data(context.decoded_buffer, true);
    return 0;
}
