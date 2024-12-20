#include <iostream>
#include <vector>

#include "BDSP/decoders/COBS.h"
#include "BDSP/encoders/COBS.h"
#include "../../tests/utils/show.h"

using namespace BDSP::decoders::COBS;
using namespace BDSP::encoders::COBS;

struct Context {
    std::vector<uint8_t> encoded_buffer;
    std::vector<uint8_t> decoded_buffer;
};

int main() {
    Context context;

    size_t size = 10;
    uint8_t data[size];
    for (int i = 0; i < size; ++i) data[i] = i;


    auto encoder = COBSEncoder({'\0', 16});
    auto decoder = COBSDecoder({'\0', 16});

    encoder.set_writer([](uint8_t byte, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "encode writer ctx_ptr is null!" << std::endl;
            return;
        };
        auto &context = *reinterpret_cast<Context *>(ctx_ptr);
        context.encoded_buffer.push_back(byte);
    }, &context);

    decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx_ptr) {
        if (not ctx_ptr) { std::cout << "decode data handler ctx_ptr is null!" << std::endl; return; }
        switch (status) {
            case DECODE_OK:
                reinterpret_cast<Context *>(ctx_ptr)->decoded_buffer.push_back(byte);
                break;
            case DECODE_END:
//                std::cout << "end" << std::endl;
                break;
            case DECODE_ERROR:
            case UNKNOWN_DECODER_ERROR:
                std::cout << "Symbol: " << uint32_t(byte) << " - ";
                show_status(status);
                break;
        }
    }, &context);


    std::cout << "Data (" << size << ") ";
    show_data(data, size, HEX);

    context.encoded_buffer.push_back(0);
    context.encoded_buffer.push_back(0);
    context.encoded_buffer.push_back(0);
    context.encoded_buffer.push_back(0);
    encoder.encode(data, size);
    encoder.finish_encode();
    context.encoded_buffer.push_back(0);

//    if (context.encoded_buffer.empty()) return 0;



    std::cout << "Encoded Buffer (" << context.encoded_buffer.size() << ") ";
    show_data(context.encoded_buffer.data(), context.encoded_buffer.size(), HEX);


    decoder.decode(context.encoded_buffer.data(), context.encoded_buffer.size());

    std::cout << "Decoded Buffer (" << context.decoded_buffer.size() << ") ";
    show_data(context.decoded_buffer.data(), context.decoded_buffer.size(), HEX);

    bool equals = context.decoded_buffer.size() == size;

    if (equals) {
        for (int i = 0; i < context.decoded_buffer.size(); ++i) {
            if (context.decoded_buffer[i] == data[i]) continue;
            equals = false;
            break;
        }
    }

    std::cout << "Equals: " << (equals ? "TRUE" : "FALSE") << std::endl;
    return 0;
}
