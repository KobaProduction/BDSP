#include <iostream>
#include <vector>

#include <BDSP/streams/PPP/reader.h>
#include <BDSP/streams/PPP/writer.h>

#include "utils.h"

using namespace BDSP::streams;
using namespace BDSP::streams::PPP;

struct Context {
    std::vector<uint8_t> write_buffer;
    std::vector<uint8_t> read_buffer;
};

int main() {
    Context context;

    std::vector<uint8_t> data = {1, 0x7E, 3, 0x7D, 0};
    std::cout << "Default ";
    show_data_(data, true);

    auto ppp_writer = PPPWriter();
    ppp_writer.set_writer([](uint8_t byte, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "read writer ctx_ptr is null!" << std::endl;
            return;
        };
        reinterpret_cast<Context *>(ctx_ptr)->write_buffer.push_back(byte);
    }, &context);

    ppp_writer.write(data.data(), data.size());

    std::cout << "Write ";
    show_data_(context.write_buffer, true);

    auto ppp_reader = PPPDecoder();
    ppp_reader.reset_read_state(false);
    ppp_reader.set_data_handler([](uint8_t byte, read_status_t state, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "read data handler ctx_ptr is null!" << std::endl;
            return;
        };
        reinterpret_cast<Context *>(ctx_ptr)->read_buffer.push_back(byte);
    }, &context);

    ppp_reader.read(context.write_buffer.data(), context.write_buffer.size());

    std::cout << "Read ";
    show_data_(context.read_buffer, true);
    return 0;
}
