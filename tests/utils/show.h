#ifndef BDSP_TESTS_UTILS_SHOW_H
#define BDSP_TESTS_UTILS_SHOW_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <BDSP/packet.h>
#include <BDSP/decoders/types.h>
#include <BDSP/encoders/types.h>

using namespace BDSP::decoders;
using namespace BDSP::encoders;

enum show_t {DEC, HEX};

void show_byte(uint8_t byte, show_t type = DEC);

void show_data(uint8_t *buf, size_t size, show_t type = DEC);

void show_data(std::vector<uint8_t> &data, show_t type = DEC);

void show_packet(BDSP::Packet &packet, show_t type = DEC);

void show_status(decode_status_t status);

void show_status(encode_status_t status);

#endif //BDSP_TESTS_UTILS_SHOW_H
