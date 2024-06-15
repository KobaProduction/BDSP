#include "bdsp_transmitter.h"

BDSPTransmitter::BDSPTransmitter() {
    _encoder = nullptr;
}

BDSPTransmitter::~BDSPTransmitter() {
    delete _encoder;
}

bdsp_set_config_status_t BDSPTransmitter::set_config(cobs_config_t config, cobs_write_handler_t handler, void *context) {
    if (_encoder) return CONFIG_ALREADY_INSTALLED;
    _encoder = new COBSEncoder(config, handler, context);
    if (_encoder->get_status() not_eq cobs_encoder_status_t::COBS_OK) {
        delete _encoder;
        _encoder = nullptr;
        return COBS_BUFFER_NOT_CREATED;
    }
    return CONFIG_INSTALLED;
}

bdsp_status_t BDSPTransmitter::send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size) {
    if (size > _max_packet_size) return BDSP_EXCESS_SIZE_PACKET;
    Packet packet = Packet(packet_id, size, buffer_ptr);
    return send_packet(packet);
}

bdsp_status_t BDSPTransmitter::send_packet(Packet &packet) {
    if (not _encoder) return BDSP_CONFIG_NOT_INSTALLED;
    if (packet.size > _max_packet_size) return BDSP_EXCESS_SIZE_PACKET;
    uint8_t checksum;
    checksum = crc8(&packet.id, 1);
    checksum = crc8(reinterpret_cast<uint8_t*>(&packet.size), 2, checksum);
    checksum = crc8(packet.data_ptr, packet.size, checksum);
    _encoder->send_segment(&packet.id, 1);
    _encoder->send_segment(reinterpret_cast<uint8_t*>(&packet.size), 2);
    _encoder->send_segment(packet.data_ptr, packet.size);
    _encoder->send_segment(&checksum, 1);
    _encoder->finish_sending();
    return WRITE_OK;
}