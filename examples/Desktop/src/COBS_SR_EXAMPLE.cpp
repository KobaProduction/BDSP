#include <iostream>
#include <vector>

#include <BDSP/streams/COBS/reader.h>
#include <BDSP/streams/COBS/writer.h>

#include "utils.h"

using namespace BDSP::streams;
using namespace BDSP::streams::COBS;

struct Context {
    std::vector<uint8_t> write_buffer;
    std::vector<uint8_t> read_buffer;
};

int main() {
    Context context;
    std::vector<uint8_t> data = {};

    for (int i = 1; i < 16; ++i) {
        data.push_back(0);
    }

    cobs_config_t config = {.delimiter = '\0', .size_of_the_sequence_to_be_replaced = 2};

    COBSWriter writer;
    writer.set_config(config);
    auto reader = COBSReader(config);

    writer.set_stream_writer([](uint8_t byte, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "read writer ctx_ptr is null!" << std::endl;
            return;
        };
        reinterpret_cast<Context *>(ctx_ptr)->write_buffer.push_back(byte);
    }, &context);

    reader.set_stream_data_handler([](uint8_t byte, read_status_t status, void *ctx_ptr) {
        if (not ctx_ptr) {
            std::cout << "read data handler ctx_ptr is null!" << std::endl;
            return;
        }
        switch (status) {
            case READ_OK:
                reinterpret_cast<Context *>(ctx_ptr)->read_buffer.push_back(byte);
                break;
            case READ_END:
//                std::cout << "end" << std::endl;
                break;
            case READ_ERROR:
            case UNKNOWN_READER_ERROR:
                std::cout << "Symbol: " << uint32_t(byte) << " - ERROR" << std::endl;
                break;
        }
    }, &context);

    writer.write(data.data(), data.size());
    writer.finish();

    std::cout << "Default Buffer ";
    show_data_(data, true);

    std::cout << "Encoded Buffer ";
    show_data_(context.write_buffer, true);

    reader.read(context.write_buffer.data(), context.write_buffer.size());

    std::cout << "Decoded Buffer ";
    show_data_(context.read_buffer, true);

    std::cout << "Size default: " << data.size() << std::endl;
    std::cout << "Size encoded: " << context.write_buffer.size() << std::endl;
    return 0;
}
