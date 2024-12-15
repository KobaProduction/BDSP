#ifndef BDSP_TESTS_UTILS_SHOW_H
#define BDSP_TESTS_UTILS_SHOW_H

#include <cstdint>
#include <iostream>
#include <BDSP/packet.h>
#include <BDSP/decoders/types.h>
#include <BDSP/encoders/types.h>

using namespace BDSP::decoders;
using namespace BDSP::encoders;

enum show_t {
    DEC, HEX
};

void show_byte(uint8_t byte, show_t type = DEC);

void show_data(uint8_t *buf, size_t size, show_t type = DEC);

void show_packet(Packet &packet, show_t type = DEC);

void show_state(decode_status_t status);

void show_state(encode_status_t status);

#endif //BDSP_TESTS_UTILS_SHOW_H
