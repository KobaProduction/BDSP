#ifndef BDSP_TESTS_UTILS_SHOW_H
#define BDSP_TESTS_UTILS_SHOW_H

#include <cstdint>
#include <iostream>
#include <vector>

#include <BDSP/packet.h>
#include <BDSP/streams/types.h>

void show_byte(uint8_t byte, bool use_hex = false);

void show_data(uint8_t *buf, size_t size, bool use_hex = false);

void show_data(std::vector<uint8_t> &data, bool use_hex = false);

void show_packet(BDSP::Packet &packet, bool use_hex = false);

void show_status(BDSP::streams::read_status_t status);

void show_status(BDSP::streams::write_status_t status);

#endif //BDSP_TESTS_UTILS_SHOW_H
