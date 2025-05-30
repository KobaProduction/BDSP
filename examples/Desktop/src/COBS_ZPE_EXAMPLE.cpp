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
    std::vector<uint8_t> test = {0x45, 0x00, 0x00, 0x2C, 0x4C, 0x79, 0x00, 0x00, 0x40, 0x06, 0x4F, 0x37};

    for (int i = 1; i < 224; ++i) {
        data.push_back(0xAA);
    }
    data.insert(data.end(), test.begin(), test.end());

    COBSZPEWriter writer;
    COBSZPEReader reader;

    writer.set_stream_writer(
        [](uint8_t byte, void *ctx_ptr) {
            if (not ctx_ptr) {
                std::cout << "read writer ctx_ptr is null!" << std::endl;
                return;
            };
            reinterpret_cast<Context *>(ctx_ptr)->write_buffer.push_back(byte);
        },
        &context);

    reader.set_stream_data_handler(
        [](uint8_t byte, read_status_t status, void *ctx_ptr) {
            if (not ctx_ptr) {
                std::cout << "read data handler ctx_ptr is null!" << std::endl;
                return;
            }
            switch (status) {
            case STREAM_READ_OK: reinterpret_cast<Context *>(ctx_ptr)->read_buffer.push_back(byte); break;
            case STREAM_READ_END:
                //                std::cout << "end" << std::endl;
                break;
            case STREAM_READ_ERROR:
            case STREAM_READER_NOT_READY_ERROR: std::cout << "Symbol: " << uint32_t(byte) << " - ERROR" << std::endl; break;
            }
        },
        &context);

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
