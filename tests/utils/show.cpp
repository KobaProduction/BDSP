#include "show.h"

using namespace BDSP;
using namespace BDSP::streams;

void show_byte(uint8_t byte, bool use_hex) {
    if (use_hex) {
        if (byte <= 0xF) {
            std::cout << 0;
        }
        std::cout << std::hex << std::uppercase;
    }
    std::cout << static_cast<uint32_t>(byte);
    std::cout << std::dec;
}

void show_data(uint8_t *buf, size_t size, bool use_hex) {
    std::cout << "Data: ";
    if (not size) {
        std::cout << "EMPTY" << std::endl;
        return;
    }
    show_byte(buf[0], use_hex);
    for (int i = 1; i < size; ++i) {
        std::cout << " ";
        show_byte(buf[i], use_hex);
    }
    std::cout << std::dec << std::endl;
}

void show_data(std::vector<uint8_t> &data, bool use_hex) {
    show_data(data.data(), data.size(), use_hex);
}

void show_packet_context(packet_context_t &packet_context, bool use_hex) {
    std::cout << "[id=" << uint32_t(packet_context.packet_id) << "] Packet(size=" << packet_context.size << "): ";
    show_data(packet_context.data_ptr, packet_context.size, use_hex);
}

void show_status(stream_read_status_t status) {
    switch (status) {
    case STREAM_READ_OK: std::cout << "STREAM_READ_OK"; break;
    case STREAM_READ_DELIMITER: std::cout << "STREAM_READ_END"; break;
    case STREAM_READ_ERROR: std::cout << "STREAM_READ_ERROR"; break;
    case STREAM_READER_NOT_READY_ERROR: std::cout << "STREAM_READER_NOT_READY_ERROR"; break;
    }
    std::cout << std::endl;
}

void show_status(stream_write_status_t status) {
    switch (status) {
    case STREAM_WRITE_OK: std::cout << "STREAM_WRITE_OK"; break;
    case STREAM_WRITE_DELIMITER: std::cout << "STREAM_WRITE_END"; break;
    case STREAM_WRITER_NOT_READY_ERROR: std::cout << "STREAM_WRITER_NOT_READY_ERROR"; break;
    }
    std::cout << std::endl;
}