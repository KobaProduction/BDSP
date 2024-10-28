#include "BDSP/bdsp_receiver.h"

BDSPReceiver::BDSPReceiver() {
    _decoder = nullptr;
    _raw_packet = nullptr;
    _error_handler = [] (BDSP::receiver_error_t error, void *context) {};
}

BDSPReceiver::~BDSPReceiver() {
    delete _raw_packet;
    delete _decoder;
}

BDSP::set_config_status_t BDSPReceiver::set_config(COBS::config_t cobs_config, BDSP::packet_handler_t handler, void *context) {
    if (_decoder) return BDSP::CONFIG_ALREADY_INSTALLED;
    _packet_handler = handler;
    _packet_handler_context = context;

    COBS::decoder_data_callback_t callback = [] (uint8_t byte, COBS::decode_state_t read_state, void *context) {
        BDSPReceiver &self = *reinterpret_cast<BDSPReceiver*>(context);
        self._parse_packet_byte(byte, read_state);
    };

    _decoder = new COBSDecoder(cobs_config, callback, this);
    return BDSP::CONFIG_INSTALLED;
}

void BDSPReceiver::set_error_handler(BDSP::receiver_error_handler_t error_handler, void *error_handler_context_ptr) {
    _error_handler = error_handler;
    _error_handler_context = error_handler_context_ptr;
}

BDSP::status_t BDSPReceiver::parse(uint8_t *buffer_ptr, size_t size) {
    if (not _decoder) return BDSP::BDSP_CONFIG_NOT_INSTALLED;
    _decoder->parse(buffer_ptr, size);
    return BDSP::PARSE_OK;
}

BDSP::status_t BDSPReceiver::parse(uint8_t &byte) {
    if (not _decoder) return BDSP::BDSP_CONFIG_NOT_INSTALLED;
    return parse(&byte, 1);
}

void BDSPReceiver::_parse_packet_byte(uint8_t byte, COBS::decode_state_t decode_state) {
    if (decode_state == COBS::DECODE_ERROR) {
        _error_handler(BDSP::ERROR_DECODING, _error_handler_context);
        return _reset();
    }

    if (_fsm_state not_eq BDSP::PACKET_CHECKSUM and _fsm_state not_eq BDSP::PACKET_ID) {
        _packet_checksum = crc8(&byte, 1, _packet_checksum);
    }

    switch (_fsm_state) {
        case BDSP::PACKET_ID:
            _raw_packet = new Packet(byte, 0);
            _packet_checksum = crc8(&byte, 1);
            _fsm_state = BDSP::SIZE_A;
            break;
        case BDSP::SIZE_A:
            _raw_packet->size = byte << 8;
            _fsm_state = BDSP::SIZE_B;
            break;
        case BDSP::SIZE_B:
            _raw_packet->size += byte;
            if (not _raw_packet->size or _raw_packet->size > _max_packet_size) {
                _error_handler(BDSP::EXCEEDING_THE_MAXIMUM_PACKET_SIZE, _error_handler_context);
                return _reset();
            }
            _raw_packet->create_buffer();
            if (not _raw_packet->data_ptr) {
                _error_handler(BDSP::NOT_ENOUGH_RAM_FOR_PACKET, _error_handler_context);
                return _reset();
            }
            _fsm_state = BDSP::PACKET_DATA;
            break;
        case BDSP::PACKET_DATA:
            _raw_packet->data_ptr[_byte_received++] = byte;
            if (_byte_received == _raw_packet->size) {
                _fsm_state = BDSP::PACKET_CHECKSUM;
            }
            break;
        case BDSP::PACKET_CHECKSUM:
            if (_packet_checksum not_eq byte) {
                _error_handler(BDSP::PACKET_CHECKSUM_DOES_NOT_MATCH, _error_handler_context);
                return _reset();
            }
            _fsm_state = BDSP::WAIT_END;
            break;
        case BDSP::WAIT_END:
            if (decode_state == COBS::DECODE_END) {
                _packet_handler(*_raw_packet, _packet_handler_context);
                delete _raw_packet;
                _raw_packet = nullptr;
            } else {
                _error_handler(BDSP::ERROR_DECODING, _error_handler_context);
            }
            _reset();
            break;
    }
}

void BDSPReceiver::_reset() {
    _decoder->reset(true);
    _fsm_state = BDSP::PACKET_ID;
    _byte_received = 0;
    if (_raw_packet) {
        delete _raw_packet;
        _raw_packet = nullptr;
    }
}