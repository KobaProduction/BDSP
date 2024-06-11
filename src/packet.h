#ifndef BDSP_PACKET_H
#define BDSP_PACKET_H

#include <stdint.h>
#include <stdlib.h>

class __attribute__((packed)) Packet {
public:
    Packet(uint8_t id, uint16_t size, bool is_need_buffer = false);
    ~Packet();
    uint8_t id;
    uint16_t size;
    uint8_t *data;
private:
    bool need_free;
};

#endif //BDSP_PACKET_H
