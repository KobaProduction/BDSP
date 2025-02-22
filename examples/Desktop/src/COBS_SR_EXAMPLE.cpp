#include <iostream>
#include <vector>

#include <BDSP/streams/cobs/reader.h>
#include <BDSP/streams/cobs/writer.h>

#include "utils.h"

using namespace BDSP::streams;
using namespace BDSP::streams::strategies::cobs;

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

    cobs_config_t config = BDSP::streams::strategies::cobs::core::COBSSRConfigsMixin().get_default_config();

    cobs::COBSSRWriterStream writer;
    writer.get_strategy().set_config(config);
    cobs::COBSSRReaderStream reader;
    reader.get_strategy().set_config(config);

    writer.set_stream_writer(
        [](uint8_t byte, void *ctx_ptr) {
            if (not ctx_ptr) {
                std::cout << "read_with_status writer ctx_ptr is null!" << std::endl;
                return;
            };
            reinterpret_cast<Context *>(ctx_ptr)->write_buffer.push_back(byte);
        },
        &context);

    reader.set_stream_data_handler(
        [](uint8_t byte, stream_read_status_t status, void *ctx_ptr) {
            if (not ctx_ptr) {
                std::cout << "read_with_status data handler ctx_ptr is null!" << std::endl;
                return;
            }
            switch (status) {
            case STREAM_READ_OK: reinterpret_cast<Context *>(ctx_ptr)->read_buffer.push_back(byte); break;
            case STREAM_READ_DELIMITER:
                //                std::cout << "end" << std::endl;
                break;
            case STREAM_READ_ERROR:
            case STREAM_READER_NOT_READY_ERROR:
                std::cout << "Symbol: " << uint32_t(byte) << " - ERROR" << std::endl;
                break;
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
