#include "bdsp_receiver.h"

BDSPReceiver::BDSPReceiver() {
    _decoder = nullptr;
    _raw_packet = nullptr;
}

BDSPReceiver::~BDSPReceiver() {
    delete _raw_packet;
}

bdsp_set_config_status_t BDSPReceiver::set_config(cobs_config_t config, packet_handler_t handler, void *context) {
    if (_decoder) return CONFIG_ALREADY_INSTALLED;
    _packet_handler = handler;
    _packet_handler_context = context;

    cobs_decoder_data_callback_t callback = [] (uint8_t byte, decode_state_t read_state, void *context) {
        BDSPReceiver &self = *reinterpret_cast<BDSPReceiver*>(context);
        self._parse_packet_byte(byte, read_state);
    };

    _decoder = new COBSDecoder(config, callback, this);
    return CONFIG_INSTALLED;
}

bdsp_status_t BDSPReceiver::parse(uint8_t *buffer_ptr, size_t size) {
    if (not _decoder) return BDSP_CONFIG_NOT_INSTALLED;
    _decoder->parse(buffer_ptr, size);
    return PARSE_OK;
}

bdsp_status_t BDSPReceiver::parse(uint8_t &byte) {
    if (not _decoder) return BDSP_CONFIG_NOT_INSTALLED;
    _decoder->parse(&byte, 1);
    return PARSE_OK;
}

void BDSPReceiver::_parse_packet_byte(uint8_t byte, decode_state_t decode_state) {
    if (decode_state == ERROR) return _reset();

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
            _raw_packet->size = byte;
            _fsm_state = SIZE_B;
            break;
        case SIZE_B:
            _raw_packet->size += byte << 8;
            if (_raw_packet->size > _max_packet_size or _raw_packet->size < 2) {
                // Packet size error.
                return _reset();
            }
            _raw_packet->create_buffer();
            if (not _raw_packet->data_ptr) {
                // Error creating packet buffer.
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
            if (_packet_checksum not_eq byte) return _reset();
            _fsm_state = WAIT_END;
            break;
        case WAIT_END:
            if (decode_state == END) {
                _packet_handler(*_raw_packet, _packet_handler_context);
            }
            _reset();
            break;
    }
}

void BDSPReceiver::_reset() {
    _decoder->reset(true);
    _fsm_state = PACKET_ID;
    _byte_received = 0;
    if (_raw_packet) {
        delete _raw_packet;
        _raw_packet = nullptr;
    }
}