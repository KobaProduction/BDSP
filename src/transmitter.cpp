#include "BDSP/transmitter.h"
#include "BDSP/checksums/crc/crc8.h"

using namespace BDSP;
using namespace BDSP::streams;
using namespace BDSP::utils;

void BDSPTransmitter::set_writer(streams::IWriter *writer_ptr) {
    // todo statuses
    _writer = writer_ptr;
}

status_t BDSPTransmitter::send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size) {
    bdsp_packet_t packet = {static_cast<uint16_t>(size), buffer_ptr};
    return send_packet(packet_id, packet);
}

status_t BDSPTransmitter::send_packet(uint8_t packet_id, bdsp_packet_t &packet, bool use_crc) {
    if (not _writer) {
        return BDSP_CONFIG_NOT_INSTALLED;
    }

    if (packet.size < 1 or packet.size > _max_packet_size) {
        return BDSP_EXCESS_SIZE_PACKET;
    }

    if (packet_id > 15) {
        return BDSP_ERROR_PACKET_ID;
    }

    core::bdsp_packet_v1_header header = {
        .unsupported_protocol_version = false,
        .two_bytes_for_packet_size_flag = packet.size > 255,
        .crc_flag = use_crc,
        .service_packet_flag = false,
        .packet_id = packet_id
    };

    uint8_t checksum = crc8(reinterpret_cast<uint8_t *>(&header), 1);

    _writer->write(reinterpret_cast<uint8_t *>(&header), 1);

    uint8_t size_byte = packet.size;

    _writer->write(&size_byte, 1);
    checksum = crc8(&size_byte, 1, checksum);

    if (header.two_bytes_for_packet_size_flag) {
        size_byte = packet.size >> 8;
        checksum = crc8(&size_byte, 1, checksum);
        _writer->write(&size_byte, 1);
    }

    _writer->write(packet.data_ptr, packet.size);

    if (use_crc) {
        checksum = crc8(packet.data_ptr, packet.size, checksum);
        _writer->write(checksum);
    }

    _writer->finish();
    return BDSP_WRITE_OK;
}
