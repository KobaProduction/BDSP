#include "packet.h"

Packet::Packet(uint8_t id_, size_t size_, uint8_t *data_ptr_) {
    id = id_;
    size = size_;
    data_ptr = data_ptr_;
}

Packet::~Packet() {
    if (need_free) free(data_ptr);
}

bool Packet::create_buffer() {
    data_ptr = reinterpret_cast<uint8_t*>(malloc(size));
    if (not data_ptr) return false;
    need_free = true;
    return true;
}