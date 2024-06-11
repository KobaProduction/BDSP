#include "BDSP.h"

BDSP::BDSP(BDSP_MODE mode_) {
    mode = mode_;
    writer = nullptr;
    reader = nullptr;
}

BDSP::~BDSP() {
    delete writer;
    delete reader;
}

bdsp_status_t BDSP::set_writer(void (*writer_ptr)(uint8_t *data_ptr, size_t size)) {
    if (writer or reader) return bdsp_status_t::BDSP_CONFIG_ALREADY_INSTALLED;
    writer_h = writer_ptr;
    return bdsp_status_t::BDSP_OK;
}

bdsp_status_t BDSP::set_config(cobs_config_t config) {
    if (writer or reader) return bdsp_status_t::BDSP_CONFIG_ALREADY_INSTALLED;
    bool need_writer = mode == BDSP_DUPLEX or mode == BDSP_SENDER;
    bool need_reader = mode == BDSP_DUPLEX or mode == BDSP_RECEIVER;
    if (need_writer and not writer_h) return bdsp_status_t::BDSP_WRITER_NOT_INSTALLED;
    if (need_writer) {
        writer = new COBSWriter(config, writer_h);
        if (writer->get_status() != cobs_writer_status_t::COBS_OK) {
            return bdsp_status_t::BDSP_ERROR_STATUS_COBS_WRITER;
        }
    }
    if (need_reader) reader = new COBSReader(config);
    return bdsp_status_t::BDSP_OK;
}

bdsp_status_t BDSP::set_max_packet_size(uint16_t max_size) {
    if (max_packet_size < 1) return bdsp_status_t::BDSP_INVALID_MAXIMUM_PACKET_SIZE;
    max_packet_size = max_size;
    return bdsp_status_t::BDSP_OK;
}

void BDSP::tick() {

}

bdsp_status_t BDSP::send_data(uint8_t id, uint8_t *data_ptr, size_t size) {
    if (size > max_packet_size) return bdsp_status_t::BDSP_EXCESS_SIZE_PACKET;
    Packet packet = Packet(id, size);
    packet.data = data_ptr;
    return send_packet(packet);;
}

bdsp_status_t BDSP::send_packet(Packet &packet) {
    if (packet.size > max_packet_size) return bdsp_status_t::BDSP_EXCESS_SIZE_PACKET;
    if (not writer) return bdsp_status_t::BDSP_CONFIG_NOT_INSTALLED;
    uint8_t checksum;
    checksum = crc8(&packet.id, 1);
    checksum = crc8(&packet.size, 2, checksum);
    checksum = crc8(packet.data, packet.size, checksum);
    writer->send_segment(&packet.id, 1);
    writer->send_segment(&packet.size, 2);
    writer->send_segment(packet.data, packet.size);
    writer->send_segment(&checksum, 1);
    writer->finish_sending();
    return bdsp_status_t::BDSP_OK;
}
