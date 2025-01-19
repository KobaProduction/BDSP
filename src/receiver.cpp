#include "BDSP/receiver.h"

using namespace BDSP;
using namespace BDSP::core;
using namespace BDSP::streams;

BDSPReceiver::BDSPReceiver() {
    _error_handler = [](receiver_error_t error, void *context) {};
}

BDSPReceiver::~BDSPReceiver() {
    delete _raw_packet;
}

void BDSPReceiver::set_reader(streams::IReader *reader_ptr) {
    _reader = reader_ptr;
    // #todo error nullptr
    if (not reader_ptr) return;
    stream_data_handler_t callback = [](uint8_t byte, read_status_t read_state, void *context) {
        reinterpret_cast<BDSPReceiver *>(context)->_parse_packet_byte(byte, read_state);
    };
    _reader->set_stream_data_handler(callback, this);
}

void BDSPReceiver::set_packet_handler(packet_handler_t packet_handler, void *context) {
    // #todo error nullptr
    _packet_handler = packet_handler;
    _packet_handler_context = context;
}

void BDSPReceiver::set_error_handler(receiver_error_handler_t error_handler, void *error_handler_context_ptr) {
    // #todo error nullptr
    _error_handler = error_handler;
    _error_handler_context = error_handler_context_ptr;
}

status_t BDSPReceiver::parse(uint8_t *buffer_ptr, size_t size) {
    if (not _reader) return BDSP_CONFIG_NOT_INSTALLED;
    _reader->read(buffer_ptr, size);
    return PARSE_OK;
}

status_t BDSPReceiver::parse(uint8_t &byte) {
    if (not _reader) return BDSP_CONFIG_NOT_INSTALLED;
    return parse(&byte, 1);
}

void BDSPReceiver::_parse_packet_byte(uint8_t byte, read_status_t decode_status) {
    if (decode_status == READ_ERROR) {
        _error_handler(ERROR_DECODING, _error_handler_context);
        return _reset();
    }

    if (_fsm_state not_eq PACKET_CHECKSUM and _fsm_state not_eq PACKET_ID) {
        _packet_checksum = crc8(&byte, 1, _packet_checksum);
    }

    switch (_fsm_state) {
        case PACKET_ID:
            _raw_packet = new Packet(byte, 0);
            _packet_checksum = crc8(&byte, 1);
            _fsm_state = SIZE_A;
            break;
        case SIZE_A:
            _raw_packet->size = byte << 8;
            _fsm_state = SIZE_B;
            break;
        case SIZE_B:
            _raw_packet->size += byte;
            if (not _raw_packet->size or _raw_packet->size > _max_packet_size) {
                _error_handler(EXCEEDING_THE_MAXIMUM_PACKET_SIZE, _error_handler_context);
                return _reset();
            }
            _raw_packet->create_buffer();
            if (not _raw_packet->data_ptr) {
                _error_handler(NOT_ENOUGH_RAM_FOR_PACKET, _error_handler_context);
                return _reset();
            }
            _fsm_state = PACKET_DATA;
            break;
        case PACKET_DATA:
            _raw_packet->data_ptr[_byte_received++] = byte;
            if (_byte_received == _raw_packet->size) {
                _fsm_state = PACKET_CHECKSUM;
            }
            break;
        case PACKET_CHECKSUM:
            if (_packet_checksum not_eq byte) {
                _error_handler(PACKET_CHECKSUM_DOES_NOT_MATCH, _error_handler_context);
                return _reset();
            }
            _fsm_state = WAIT_END;
            break;
        case WAIT_END:
            if (decode_status == READ_END) {
                _packet_handler(*_raw_packet, _packet_handler_context);
                delete _raw_packet;
                _raw_packet = nullptr;
            } else {
                _error_handler(ERROR_DECODING, _error_handler_context);
            }
            _reset();
            break;
    }
}

void BDSPReceiver::_reset() {
    _reader->reset_read_state(true);
    _fsm_state = PACKET_ID;
    _byte_received = 0;
    if (_raw_packet) {
        delete _raw_packet;
        _raw_packet = nullptr;
    }
}

