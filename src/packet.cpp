#include "./packet.h"

Packet::Packet(uint8_t id_, size_t size_, uint8_t *data_ptr) {
    id = id_;
    size = size_;
    data = data_ptr;
}

Packet::~Packet() {
    if (need_free) free(data);
}

bool Packet::create_buffer() {
    data = reinterpret_cast<uint8_t*>(malloc(size));
    if (not data) return false;
    need_free = true;
    return true;
}