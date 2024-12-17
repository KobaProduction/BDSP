#include <iostream>
#include "cobs.h"

uint8_t *cobs_decode(const uint8_t *ptr, size_t length, uint8_t *dst) {
    const uint8_t *end = ptr + length;
    while (ptr < end) {
        if (*ptr == 0x00) break;
        int i, code = *ptr++;
        for (i = 1; i < code; i++) *dst++ = *ptr++;
        if (code < 0xFF) *dst++ = 0;
    }
    return --dst;
}

void cobs_decode(std::vector<uint8_t> &encoded_buffer, std::vector<uint8_t> &output_buffer) {
    uint8_t buffer[encoded_buffer.size()];
    uint8_t *end_ptr = cobs_decode(encoded_buffer.data(), encoded_buffer.size(), buffer);
    for (int i = 0; i < end_ptr - buffer; ++i) output_buffer.push_back(buffer[i]);
}

uint8_t *cobs_encode(const uint8_t *ptr, size_t length, uint8_t *dst) {
    const uint8_t *end = ptr + length;
    uint8_t *code_ptr = dst++;
    uint8_t code = 0x01;
    while (ptr < end) {
        if (*ptr == 0) {
            *code_ptr = code;
            code_ptr = dst++;
            code = 0x01;
        } else {
            *dst++ = *ptr;
            code++;
            if (code == 0xFF) {
                *code_ptr = code;
                code_ptr = dst++;
                code = 0x01;
            }
        }
        ptr++;
    }
    *code_ptr = code;
    return dst;
}

void cobs_encode(std::vector<uint8_t> &data, std::vector<uint8_t> &encoded_buffer) {
    uint8_t buffer[10 + data.size() + data.size() / 10];
    uint8_t *end_ptr = cobs_encode(data.data(), data.size(), buffer);
    for (int i = 0; i < end_ptr - buffer; ++i) encoded_buffer.push_back(buffer[i]);
    encoded_buffer.push_back(0x00);
}
