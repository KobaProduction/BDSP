#ifndef BDSP_PACKET_H
#define BDSP_PACKET_H

#ifdef ARDUINO
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#else
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#endif

class __attribute__((packed)) Packet {
public:
    Packet(uint8_t id, size_t size, uint8_t *data_ptr = nullptr);
    ~Packet();
    bool create_buffer();
    uint8_t id = 0;
    uint16_t size = 0;
    uint8_t *data = nullptr;
private:
    bool need_free = false;
};

#endif //BDSP_PACKET_H