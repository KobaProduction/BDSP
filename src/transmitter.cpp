#include "BDSP/transmitter.h"

using namespace BDSP;
using namespace streams;

void BDSPTransmitter::set_writer(streams::IWriter *writer_ptr) {
    // todo statuses
    _writer = writer_ptr;
}

status_t BDSPTransmitter::send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size) {
    if (size > _max_packet_size) return BDSP_EXCESS_SIZE_PACKET;
    Packet packet = Packet(packet_id, size, buffer_ptr);
    return send_packet(packet);
}

status_t BDSPTransmitter::send_packet(Packet &packet) {
    if (not _writer) return BDSP_CONFIG_NOT_INSTALLED;
    if (packet.size < 1 or packet.size > _max_packet_size) return BDSP_EXCESS_SIZE_PACKET;
    uint8_t checksum;
    uint8_t size_byte;
    checksum = crc8(&packet.id, 1);
    _writer->write(packet.id);
    size_byte = packet.size >> 8;
    checksum = crc8(&size_byte, 1, checksum);
    _writer->write(&size_byte, 1);
    size_byte = packet.size;
    checksum = crc8(&size_byte, 1, checksum);
    _writer->write(&size_byte, 1);
    checksum = crc8(packet.data_ptr, packet.size, checksum);
    _writer->write(packet.data_ptr, packet.size);
    _writer->write(checksum);
    _writer->finish();
    return WRITE_OK;
}



