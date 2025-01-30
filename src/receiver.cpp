#include "BDSP/receiver.h"
#include "BDSP/checksums/crc8.h"

using namespace BDSP;
using namespace BDSP::core;
using namespace BDSP::streams;
using namespace BDSP::checksums;

BDSPV1Receiver::BDSPV1Receiver() {
    _error_handler = nullptr;
}

BDSPV1Receiver::~BDSPV1Receiver() {
    _reset();
    //    free(_raw_packet.data_ptr);
}

void BDSPV1Receiver::set_stream_reader(streams::IReader *reader_ptr) {
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

void BDSPV1Receiver::_allocate_packet_memory() {
    if (not _packet_context.size or _packet_context.size > _max_packet_size) {
        return _handle_error(EXCEEDING_THE_MAXIMUM_PACKET_SIZE);
    }
    _packet_context.data_ptr = static_cast<uint8_t *>(malloc(_packet_context.size));
    if (not _packet_context.data_ptr) {
        return _handle_error(NOT_ENOUGH_RAM_FOR_PACKET);
    }
}

void BDSPV1Receiver::_deallocate_packet_memory() {
    free(_packet_context.data_ptr);
    _packet_context.data_ptr = nullptr;
}

void BDSPV1Receiver::_handle_error(receiver_error_t error) {
    if (_error_handler) {
        _error_handler(error, _error_handler_context);
    }
    _reset();
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
    case PACKET_DATA:
        _packet_context.data_ptr[_received_packet_data_bytes++] = byte;
        if (_received_packet_data_bytes == _packet_context.size) {
            _fsm_state = PACKET_CHECKSUM;
        }
        return;
    case PACKET_HEADER:
        _packet_header = *reinterpret_cast<bdsp_packet_v1_header *>(&byte);
        if (_packet_header.is_unsupported_protocol_version) {
            return _handle_error(UNSUPPORTED_PROTOCOL);
        }
        if (_packet_header.is_service_packet) {
            return _handle_error(UNSUPPORTED_SERVICE_PACKETS);
        }
        _packet_context = {_packet_header.packet_id, nullptr, 0, false};
        _fsm_state = PACKET_SIZE_A;
        return;
    case PACKET_CHECKSUM:
        if (_calc_checksum(_packet_header, _packet_context.data_ptr, _packet_context.size) not_eq byte) {
            return _handle_error(CHECKSUM_ERROR);
        }
        _fsm_state = WAIT_END;
        return;
    case WAIT_END:
        if (decode_status == STREAM_READ_END) {
            _packet_handler(_packet_context, _packet_handler_context);
            if (not _packet_context.need_clear) {
                _deallocate_packet_memory();
            }
            return _reset();
        }
        return _handle_error(ERROR_STREAM_READING);
    case PACKET_SIZE_A:
        _packet_context.size = byte;
        if (_packet_header.is_two_bytes_for_packet_size) {
            _fsm_state = PACKET_SIZE_B;
        } else {
            _fsm_state = PACKET_DATA;
            _allocate_packet_memory();
        }
        return;
    case PACKET_SIZE_B:
        _packet_context.size = (_packet_context.size << 8) + byte;
        _fsm_state = PACKET_DATA;
        _allocate_packet_memory();
        return;
    }
}

void BDSPV1Receiver::_reset() {
    _reader->reset_read_state(true);
    _fsm_state = PACKET_HEADER;
    _received_packet_data_bytes = 0;
    if (_packet_context.data_ptr) {
        _deallocate_packet_memory();
    }
}


