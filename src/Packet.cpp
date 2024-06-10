#include "Packet.h"

Packet::~Packet() {
    if (need_free) free(data);
}

Packet::Packet(uint8_t id_, uint16_t size_, bool is_need_buffer) {
    id = id_;
    size = size_;
    if (is_need_buffer)
        data = reinterpret_cast<uint8_t*>(malloc(size_));
    need_free = is_need_buffer;
}
