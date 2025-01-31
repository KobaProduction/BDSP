#ifndef BDSP_TESTS_UTILS_SHOW_H
#define BDSP_TESTS_UTILS_SHOW_H

#include <cstdint>
#include <iostream>
#include <BDSP/packet.h>
#include <BDSP/decoders/types.h>
#include <BDSP/encoders/types.h>

using namespace BDSP::decoders;
using namespace BDSP::encoders;

enum show_t {DEC, HEX};

void show_byte(uint8_t byte, show_t type = DEC) {
    switch (type) {
        case DEC:
            std::cout << static_cast<uint32_t>(byte);
            break;
        case HEX:
            std::cout << std::hex << std::uppercase << static_cast<uint32_t>(byte);
            break;
    }
};

void show_data(uint8_t *buf, size_t size, show_t type = DEC) {
    std::cout << "Data: ";
    show_byte(buf[0], type);
    for (int i = 1; i < size; ++i) {
        std::cout << " ";
        show_byte(buf[i], type);
    }
    std::cout << std::dec << std::endl;
};

void show_packet(Packet &packet, show_t type = DEC) {
    std::cout << "[id=" << uint32_t(packet.id) << "] Packet(size=" << packet.size << "): ";
    show_data(packet.data_ptr, packet.size, type);
};

void show_state(decode_status_t status) {
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
        case DATA_HANDLER_NOT_INSTALLED:
            std::cout << "DATA_HANDLER_NOT_INSTALLED";
            break;
    }
    std::cout << std::endl;
};
void show_state(encode_status_t status) {
    switch (status) {
        case ENCODE_OK:
            std::cout << "ENCODE_OK";
            break;
        case ENCODE_FINISH:
            std::cout << "ENCODE_FINISH";
            break;
        case WRITER_NOT_INSTALLED_ERROR:
            std::cout << "WRITER_NOT_INSTALLED_ERROR";
            break;
        case UNKNOWN_ENCODER_ERROR:
            std::cout << "UNKNOWN_ENCODER_ERROR";
            break;
    }
    std::cout << std::endl;
};

#endif //BDSP_TESTS_UTILS_SHOW_H
