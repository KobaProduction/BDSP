#include "BDSP/transmitter.h"
#include "BDSP/checksums/crc8.h"

using namespace BDSP;
using namespace BDSP::core;
using namespace BDSP::streams;
using namespace BDSP::checksums;

bdsp_transmitter_send_packet_status_t BDSPV1TransmitterCore::send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size,
                                                                       checksum_usage_state_t checksum_state) {
    bdsp_packet_context_t packet_context = {packet_id, buffer_ptr, static_cast<uint16_t>(size)};
    return send_packet(packet_context, checksum_state);
}

bdsp_transmitter_send_packet_status_t BDSPV1TransmitterCore::send_packet(bdsp_packet_context_t &packet_context,
                                                                         checksum_usage_state_t checksum_state) {
    if (not _writer) {
        return STREAM_WRITER_NOT_SET_ERROR;
    }

    if (packet_context.size < 1 or packet_context.size > _max_packet_size) {
        return MAXIMUM_PACKET_SIZE_EXCEEDING_ERROR;
    }

    if (packet_context.packet_id > 15) {
        return PACKET_ID_ERROR;
    }

    bool is_need_use_checksum =
        checksum_state == DEFAULT_CHECKSUM ? _checksum_usage_default_state : checksum_state == WITH_CHECKSUM;

    core::bdsp_packet_v1_header header = {.is_unsupported_protocol_version = false,
                                          .is_two_bytes_for_packet_size = packet_context.size > 255,
                                          .is_checksum_used = is_need_use_checksum,
                                          .is_service_packet = false,
                                          .packet_id = packet_context.packet_id};

    uint8_t checksum = _calc_checksum(header, packet_context.data_ptr, packet_context.size);

    _writer->write(reinterpret_cast<uint8_t *>(&header), 1);

    uint8_t size_byte = packet_context.size;

    if (header.is_two_bytes_for_packet_size) {
        size_byte = packet_context.size >> 8;
        _writer->write(&size_byte, 1);
    }

    size_byte = packet_context.size;
    _writer->write(&size_byte, 1);

    _writer->write(packet_context.data_ptr, packet_context.size);

    if (header.is_checksum_used) {
        _writer->write(checksum);
    }

    _writer->finish();
    return SEND_PACKET_OK;
}

void BDSPV1TransmitterCore::set_checksum_usage_default_state(bool using_checksum) {
    _checksum_usage_default_state = using_checksum;
}

void BDSPV1TransmitterCore::set_stream_writer(streams::IWriter *writer_ptr) {
    // todo statuses
    _writer = writer_ptr;
}