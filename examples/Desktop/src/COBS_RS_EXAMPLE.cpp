#include <iostream>
#include <vector>

#include <BDSP/decoders/COBS.h>
#include <BDSP/encoders/COBS.h>
#include "utils.h"

using namespace BDSP::decoders::COBS;
using namespace BDSP::encoders::COBS;

struct Context {
    std::vector<uint8_t> encoded_buffer;
    std::vector<uint8_t> decoded_buffer;
};

int main() {
    Context context;
    std::vector<uint8_t> data = {};

    for (int i = 1; i < 16; ++i) {
        data.push_back(0);
    }

    auto encoder = COBSEncoder({'\0', 255, 2});
    auto decoder = COBSDecoder({'\0', 255, 2});

    encoder.set_writer([](uint8_t byte, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "encode writer ctx_ptr is null!" << std::endl;
            return;
        };
        reinterpret_cast<Context *>(ctx_ptr)->encoded_buffer.push_back(byte);
    }, &context);

    decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "decode data handler ctx_ptr is null!" << std::endl;
            return;
        }
        switch (status) {
            case DECODE_OK:
                reinterpret_cast<Context *>(ctx_ptr)->decoded_buffer.push_back(byte);
                break;
            case DECODE_END:
//                std::cout << "end" << std::endl;
                break;
            case DECODE_ERROR:
            case UNKNOWN_DECODER_ERROR:
                std::cout << "Symbol: " << uint32_t(byte) << " - ERROR" << std::endl;
                break;
        }
    }, &context);

    encoder.encode(data.data(), data.size());
    encoder.finish_encode();

    std::cout << "Default Buffer ";
    show_data(data, true);

    std::cout << "Encoded Buffer ";
    show_data(context.encoded_buffer, true);

    decoder.decode(context.encoded_buffer.data(), context.encoded_buffer.size());

    std::cout << "Decoded Buffer ";
    show_data(context.decoded_buffer, true);

    std::cout << "Size default: " << data.size() << std::endl;
    std::cout << "Size encoded: " << context.encoded_buffer.size() << std::endl;
    return 0;
}
