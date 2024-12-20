#include "show.h"

void show_byte(uint8_t byte, show_t type) {
    switch (type) {
        case DEC:
            std::cout << static_cast<uint32_t>(byte);
            break;
        case HEX:
            std::cout << std::hex << std::uppercase << static_cast<uint32_t>(byte);
            break;
    }
}

void show_data(uint8_t *buf, size_t size, show_t type) {
    std::cout << "Data: ";
    if (not size) {
        std::cout << "EMPTY" << std::endl;
        return;
    }
    show_byte(buf[0], type);
    for (int i = 1; i < size; ++i) {
        std::cout << " ";
        show_byte(buf[i], type);
    }
    std::cout << std::dec << std::endl;
}

void show_data(std::vector<uint8_t> &data, show_t type) {
    show_data(data.data(), data.size(), type);
}

void show_packet(Packet &packet, show_t type) {
    std::cout << "[id=" << uint32_t(packet.id) << "] Packet(size=" << packet.size << "): ";
    show_data(packet.data_ptr, packet.size, type);
}

void show_status(decode_status_t status) {
    switch (status) {
        case DECODE_OK:
            std::cout << "DECODE_OK";
            break;
        case DECODE_END:
            std::cout << "DECODE_END";
            break;
        case DECODE_ERROR:
            std::cout << "DECODE_ERROR";
            break;
        case UNKNOWN_DECODER_ERROR:
            std::cout << "UNKNOWN_DECODER_ERROR";
            break;
    }
    std::cout << std::endl;
}

void show_status(encode_status_t status) {
    switch (status) {
        case ENCODE_OK:
            std::cout << "ENCODE_OK";
            break;
        case ENCODE_FINISH:
            std::cout << "ENCODE_FINISH";
            break;
        case UNKNOWN_ENCODER_ERROR:
            std::cout << "UNKNOWN_ENCODER_ERROR";
            break;
    }
    std::cout << std::endl;
}
