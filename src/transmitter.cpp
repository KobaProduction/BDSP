#include "BDSP/transmitter.h"
#include "BDSP/checksums/crc8.h"

using namespace BDSP;
using namespace BDSP::core;
using namespace BDSP::streams;
using namespace BDSP::checksums;

send_packet_status_t BDSPV1TransmitterCore::send_data(uint8_t packet_id, uint8_t *buffer_ptr, size_t size) {
    return send_data(packet_id, buffer_ptr, size, _checksum_usage_default_state);
}

send_packet_status_t BDSPV1TransmitterCore::send_data(uint8_t packet_id,
                                                      uint8_t *buffer_ptr,
                                                      size_t size,
                                                      bool checksum_enforcement) {
    packet_context_t packet_context;
    packet_context.packet_id = packet_id;
    packet_context.data_ptr = buffer_ptr;
    packet_context.size = static_cast<uint16_t>(size);
    return send_packet(packet_context, checksum_enforcement);
}

send_packet_status_t BDSPV1TransmitterCore::send_packet(packet_context_t &packet_context) noexcept {
    return send_packet(packet_context, _checksum_usage_default_state);
}

send_packet_status_t BDSPV1TransmitterCore::send_packet(packet_context_t &packet_context,
                                                        bool checksum_enforcement) noexcept {
    if (not _writer) {
        return STREAM_WRITER_NOT_SET_ERROR;
    }
    if (packet_context.size < 1 or packet_context.size > _max_packet_size) {
        return MAXIMUM_PACKET_SIZE_EXCEEDING_ERROR;
    }
    if (packet_context.packet_id > 31) {
        return PACKET_ID_ERROR;
    }
    core::packet_v1_header header = {
        .packet_id = packet_context.packet_id,
        .is_two_bytes_for_packet_size = packet_context.size > 255,
        .is_checksum_used = checksum_enforcement,
        .is_unsupported_protocol_version = false,
    };
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

set_stream_writer_status_t BDSPV1TransmitterCore::set_stream_writer(streams::IStreamWriter *writer_ptr) {
    if (writer_ptr and not writer_ptr->get_ready_status()) {
        return STREAM_WRITER_NOT_READY;
    }
    _writer = writer_ptr;
    return _writer ? SET_STREAM_WRITER_OK : RESET_STREAM_WRITER;
}