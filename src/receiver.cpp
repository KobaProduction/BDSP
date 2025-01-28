#include "BDSP/receiver.h"
#include "BDSP/checksums/crc8.h"

using namespace BDSP;
using namespace BDSP::core;
using namespace BDSP::streams;
using namespace BDSP::checksums;

BDSPV1Receiver::BDSPV1Receiver() {
    _error_handler = [](receiver_error_t error, void *context) { };
}

BDSPV1Receiver::~BDSPV1Receiver() {
    _reset();
    //    free(_raw_packet.data_ptr);
}

void BDSPV1Receiver::set_reader(streams::IReader *reader_ptr) {
    _reader = reader_ptr;
    // #todo error nullptr
    if (not reader_ptr) {
        return;
    }
    stream_data_handler_t callback = [](uint8_t byte, read_status_t read_state, void *context) {
        reinterpret_cast<BDSPV1Receiver *>(context)->_parse_packet_byte(byte, read_state);
    };
    _reader->set_stream_data_handler(callback, this);
}

void BDSPV1Receiver::set_packet_handler(packet_handler_t packet_handler, void *context) {
    // #todo error nullptr
    _packet_handler = packet_handler;
    _packet_handler_context = context;
}

// void BDSPV1Receiver::set_error_handler(receiver_error_handler_t error_handler, void *error_handler_context_ptr) {
//     // #todo error nullptr
//     _error_handler = error_handler;
//     _error_handler_context = error_handler_context_ptr;
// }

status_t BDSPV1Receiver::parse(uint8_t *buffer_ptr, size_t size) {
    if (not _reader) {
        return BDSP_CONFIG_NOT_INSTALLED;
    }
    _reader->read(buffer_ptr, size);
    return BDSP_PARSE_OK;
}

status_t BDSPV1Receiver::parse(uint8_t &byte) {
    return parse(&byte, 1);
}

void BDSPV1Receiver::_parse_packet_byte(uint8_t byte, read_status_t decode_status) {
    if (decode_status == STREAM_READ_ERROR) {
        return _handle_error(ERROR_STREAM_READING);
    }

    if (decode_status == STREAM_READ_END and _fsm_state not_eq WAIT_END) {
        _handle_error(ERROR_STREAM_READING);
        _reader->reset_read_state(false);
        return;
    }

    switch (_fsm_state) {
    case PACKET_HEADER:
        _packet_header = *reinterpret_cast<bdsp_packet_v1_header *>(&byte);
        if (_packet_header.unsupported_protocol_version) {
            return _handle_error(UNSUPPORTED_PROTOCOL);
        }
        _raw_packet = {0, nullptr};
        break;
    case PACKET_SIZE_A: _raw_packet.size = byte; break;
    case PACKET_SIZE_B: _raw_packet.size = (_raw_packet.size << 8) + byte; break;
    case PACKET_DATA:
        _raw_packet.data_ptr[_byte_received++] = byte;
        if (_byte_received == _raw_packet.size) {
            _fsm_state = PACKET_CHECKSUM;
        }
        break;
    case PACKET_CHECKSUM:
        if (_calc_checksum(_packet_header, _raw_packet.data_ptr, _raw_packet.size) not_eq byte) {
            return _handle_error(CHECKSUM_ERROR);
        }
        _fsm_state = WAIT_END;
        break;
    case WAIT_END:
        if (decode_status == STREAM_READ_END) {
            bdsp_packet_context_t packet_context;
            packet_context.packet_id = _packet_header.packet_id;
            packet_context.packet = _raw_packet;
            _packet_handler(packet_context, _packet_handler_context);
            if (not packet_context.need_clear) {
                free(_raw_packet.data_ptr);
                _raw_packet.data_ptr = nullptr;
            }
            return _reset();
        }
        return _handle_error(ERROR_STREAM_READING);
    }

    if ((_fsm_state == PACKET_SIZE_A and not _packet_header.two_bytes_for_packet_size_flag) or
        _fsm_state == PACKET_SIZE_B) {
        if (not _raw_packet.size or _raw_packet.size > _max_packet_size) {
            return _handle_error(EXCEEDING_THE_MAXIMUM_PACKET_SIZE);
        }
        _raw_packet.data_ptr = static_cast<uint8_t *>(malloc(_raw_packet.size));
        if (not _raw_packet.data_ptr) {
            return _handle_error(NOT_ENOUGH_RAM_FOR_PACKET);
        }
        _fsm_state = PACKET_DATA;
    } else if (_fsm_state == PACKET_SIZE_A) {
        _fsm_state = PACKET_SIZE_B;
    } else if (_fsm_state == PACKET_HEADER) {
        _fsm_state = PACKET_SIZE_A;
    }
}

void BDSPV1Receiver::_reset() {
    _reader->reset_read_state(true);
    _fsm_state = PACKET_HEADER;
    _byte_received = 0;
    if (_raw_packet.data_ptr) {
        free(_raw_packet.data_ptr);
        _raw_packet.data_ptr = nullptr;
    }
}

void BDSPV1Receiver::_handle_error(receiver_error_t error) {
    if (_error_handler) {
        _error_handler(error, _error_handler_context);
    }
    _reset();
}
