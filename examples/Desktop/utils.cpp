#include "utils.h"

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
    show_byte(buf[0], type);
    for (int i = 1; i < size; ++i) {
        if (1) {
            std::cout << " ";
        }
        show_byte(buf[i], type);

    }
    std::cout << std::endl;
}

void show_packet(Packet &packet, show_t type) {
    std::cout << "[id=" << uint32_t(packet.id) << "] Packet(size=" << packet.size << "): ";
//    std::cout << uint32_t(packet.id) << " ";
//    auto *size_p = (uint8_t*)(&packet.size);
//    std::cout << static_cast<uint32_t>(size_p[0]) << " ";
//    std::cout << static_cast<uint32_t>(size_p[1]) << " ";
    show_data(packet.data_ptr, packet.size, type);
}

void show_state(decode_state_t state) {
    switch (state) {
        case DECODE_OK:
            std::cout << "DECODE_OK";
            break;
        case DECODE_END:
            std::cout << "DECODE_END";
            break;
        case DECODE_ERROR:
            std::cout << "DECODE_ERROR";
            break;
        case WAITING_FOR_DELIMITER:
            std::cout << "WAITING_FOR_DELIMITER";
            break;
        case DATA_HANDLER_NOT_INSTALLED:
            std::cout << "DATA_HANDLER_NOT_INSTALLED";
            break;
    }
    std::cout << std::endl;
}
