#include <BDSP/transmitter.h>

using namespace BDSP;
using namespace COBS;


BDSPTransmitter::BDSPTransmitter() {
    _encoder = nullptr;
}

BDSPTransmitter::~BDSPTransmitter() {
    delete _encoder;
}

set_config_status_t BDSPTransmitter::set_config(config_t cobs_config, write_handler_t handler, void *context) {
    if (_encoder) return CONFIG_ALREADY_INSTALLED;
    _encoder = new COBSEncoder(cobs_config, handler, context);
    if (_encoder->get_status() not_eq COBS_OK) {
        delete _encoder;
        _encoder = nullptr;
        return COBS_BUFFER_NOT_CREATED;
    }
    return CONFIG_INSTALLED;
}

status_t BDSPTransmitter::send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size) {
    if (size > _max_packet_size) return BDSP_EXCESS_SIZE_PACKET;
    Packet packet = Packet(packet_id, size, buffer_ptr);
    return send_packet(packet);
}

status_t BDSPTransmitter::send_packet(Packet &packet) {
    if (not _encoder) return BDSP_CONFIG_NOT_INSTALLED;
    if (packet.size < 1 or packet.size > _max_packet_size) return BDSP_EXCESS_SIZE_PACKET;
    uint8_t checksum;
    uint8_t size_byte;
    checksum = crc8(&packet.id, 1);
    _encoder->send_byte(packet.id);
    size_byte = packet.size >> 8;
    checksum = crc8(&size_byte, 1, checksum);
    _encoder->send_segment(&size_byte, 1);
    size_byte = packet.size;
    checksum = crc8(&size_byte, 1, checksum);
    _encoder->send_segment(&size_byte, 1);
    checksum = crc8(packet.data_ptr, packet.size, checksum);
    _encoder->send_segment(packet.data_ptr, packet.size);
    _encoder->send_byte(checksum);
    _encoder->finish_encoding();
    return WRITE_OK;
}