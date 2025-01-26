#include "show.h"

using namespace BDSP::streams;

void show_byte(uint8_t byte, bool use_hex) {
    if (use_hex) {
        if (byte <= 0xF) std::cout << 0;
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

void show_packet(BDSP::Packet &packet, bool use_hex) {
    std::cout << "[id=" << uint32_t(packet.id) << "] Packet(size=" << packet.size << "): ";
    show_data(packet.data_ptr, packet.size, use_hex);
}

void show_status(read_status_t status) {
    switch (status) {
    case STREAM_READ_OK: std::cout << "STREAM_READ_OK"; break;
    case STREAM_READ_END: std::cout << "STREAM_READ_END"; break;
    case STREAM_READ_ERROR: std::cout << "STREAM_READ_ERROR"; break;
    case ERROR_READ_STREAM_NOT_READY: std::cout << "ERROR_READ_STREAM_NOT_READY"; break;
    }
    std::cout << std::endl;
}

void show_status(write_status_t status) {
    switch (status) {
        case WRITE_OK:
            std::cout << "WRITE_OK";
            break;
        case WRITE_FINISH:
            std::cout << "WRITE_FINISH";
            break;
        case UNKNOWN_WRITER_ERROR:
            std::cout << "UNKNOWN_WRITER_ERROR";
            break;
    }
    std::cout << std::endl;
}
