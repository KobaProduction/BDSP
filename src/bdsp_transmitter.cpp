#include "bdsp_transmitter.h"

BDSPTransmitter::BDSPTransmitter() {
    encoder = nullptr;
}

BDSPTransmitter::~BDSPTransmitter() {
    delete encoder;
}

bdsp_set_config_status BDSPTransmitter::set_config(cobs_config_t config, write_handler_t handler, void *context) {
    if (encoder) return CONFIG_ALREADY_INSTALLED;
    encoder = new COBSEncoder(config, handler, context);
    if (encoder->get_status() not_eq cobs_encoder_status::COBS_OK) {
        delete encoder;
        encoder = nullptr;
        return COBS_BUFFER_NOT_CREATED;
    }
    return CONFIG_INSTALLED;
}

bdsp_transmit_status BDSPTransmitter::send_data(uint8_t id, uint8_t *data_ptr, size_t size) {
    if (size > max_packet_size) return BDSP_EXCESS_SIZE_PACKET;
    Packet packet = Packet(id, size, data_ptr);
    return send_packet(packet);;
}

bdsp_transmit_status BDSPTransmitter::send_packet(Packet &packet) {
    if (not encoder) return BDSP_CONFIG_NOT_INSTALLED;
    if (packet.size > max_packet_size) return BDSP_EXCESS_SIZE_PACKET;
    uint8_t checksum;
    checksum = crc8(&packet.id, 1);
    checksum = crc8( (uint8_t*)(&packet.size), 2, checksum);
    checksum = crc8(packet.data, packet.size, checksum);
    encoder->send_segment(&packet.id, 1);
    encoder->send_segment((uint8_t*)(&packet.size), 2);
    encoder->send_segment(packet.data, packet.size);
    encoder->send_segment(&checksum, 1);
    encoder->finish_sending();
    return WRITE_OK;
}