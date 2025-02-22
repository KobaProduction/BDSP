#ifndef BDSP_TESTS_UTILS_SHOW_H
#define BDSP_TESTS_UTILS_SHOW_H

#include <cstdint>
#include <iostream>
#include <vector>

#include <BDSP/types.h>
#include <BDSP/streams/types.h>

void show_byte(uint8_t byte, bool use_hex = false);

void show_data(uint8_t *buf, size_t size, bool use_hex = false);

void show_data(std::vector<uint8_t> &data, bool use_hex = false);

void show_packet_context(BDSP::packet_context_t &packet_context, bool use_hex = false);

void show_status(BDSP::streams::stream_read_status_t status);

void show_status(BDSP::streams::stream_write_status_t status);

#endif //BDSP_TESTS_UTILS_SHOW_H
