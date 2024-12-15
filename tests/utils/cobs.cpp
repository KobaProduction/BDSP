#include "cobs.h"

void UnStuffData(const unsigned char *ptr, unsigned long length, unsigned char *dst) {
    const unsigned char *end = ptr + length;
    while (ptr < end) {
        int i, code = *ptr++;
        for (i = 1; i < code; i++) *dst++ = *ptr++;
        if (code < 0xFF) *dst++ = 0;
    }
}


void StuffData(const unsigned char *ptr, unsigned long length, unsigned char *dst) {
    const unsigned char *end = ptr + length;
    unsigned char *code_ptr = dst++;
    unsigned char code = 0x01;
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
}