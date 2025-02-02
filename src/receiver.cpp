#include "BDSP/receiver.h"
#include "BDSP/checksums/crc8.h"

using namespace BDSP;
using namespace BDSP::core;
using namespace BDSP::streams;
using namespace BDSP::checksums;

void BDSPV1ReceiverCore::_deallocate_packet_memory() {
    _free(_packet_context.data_ptr);
    _packet_context.data_ptr = nullptr;
}

parse_packet_status_t BDSPV1ReceiverCore::_cause_error(parse_packet_status_t status) {
    if (_error_handler) {
        _error_handler(status, _error_handler_context);
    }
    reset(true);
    return status;
}

BDSPV1ReceiverCore::~BDSPV1ReceiverCore() {
    if (_reader) {
        _reader->set_stream_data_handler(nullptr, nullptr);
    }
    _deallocate_packet_memory();
}

parse_packet_status_t BDSPV1ReceiverCore::parse_packet_byte(uint8_t byte, streams::read_status_t stream_status) {
    if (stream_status == STREAM_READ_ERROR) {
        return _cause_error(STREAM_READING_ERROR);
    }
    if (stream_status == STREAM_READ_END and _fsm_state not_eq WAIT_END) {
        _cause_error(STREAM_READING_ERROR);
        reset(false);
        return PARSE_PACKET_OK;
    }
    switch (_fsm_state) {
    case PACKET_DATA:
        _packet_context.data_ptr[_received_packet_data_bytes++] = byte;
        if (_received_packet_data_bytes == _packet_context.size) {
            _fsm_state = PACKET_CHECKSUM;
        }
        return PARSE_PACKET_OK;
    case PACKET_HEADER:
        _packet_header = *reinterpret_cast<packet_v1_header *>(&byte);
        if (_packet_header.is_unsupported_protocol_version) {
            return _cause_error(UNSUPPORTED_PROTOCOL_ERROR);
        }
        _packet_context = {_packet_header.packet_id, nullptr, 0, true};
        _fsm_state = PACKET_SIZE_A;
        return PARSE_PACKET_OK;
    case PACKET_CHECKSUM:
        if (_calc_checksum(_packet_header, _packet_context.data_ptr, _packet_context.size) not_eq byte) {
            return _cause_error(CHECKSUM_ERROR);
        }
        _fsm_state = WAIT_END;
        return PARSE_PACKET_OK;
    case WAIT_END:
        if (stream_status not_eq STREAM_READ_END) {
            return _cause_error(STREAM_READING_ERROR);
        }
        if (not _packet_header.is_service_packet) {
            if (not _packet_handler) {
                return _cause_error(PACKET_HANDLER_NOT_SET_ERROR);
            }
            _packet_handler(_packet_context, _packet_handler_context);
        } else if (not _service_packet_handler) {
            return _cause_error(SERVICE_PACKET_HANDLER_NOT_SET_ERROR);
        } else {
            _service_packet_handler(_packet_context, _service_packet_handler_context);
        }

        if (not _packet_context.need_clear) {
            _packet_context.data_ptr = nullptr;
        }

        reset(false);
        return PARSE_PACKET_OK;
    case PACKET_SIZE_A:
        _packet_context.size = byte;
        if (_packet_header.is_two_bytes_for_packet_size) {
            _fsm_state = PACKET_SIZE_B;
            return PARSE_PACKET_OK;
        }
        break;
    case PACKET_SIZE_B: _packet_context.size = (_packet_context.size << 8) + byte; break;
    }

    if (not _packet_context.size or _packet_context.size > _max_packet_size) {
        return _cause_error(EXCEEDING_THE_MAXIMUM_PACKET_SIZE_ERROR);
    }
    _packet_context.data_ptr = static_cast<uint8_t *>(_malloc(_packet_context.size));
    if (not _packet_context.data_ptr) {
        return _cause_error(NOT_ENOUGH_RAM_FOR_PACKET_ERROR);
    }
    _fsm_state = PACKET_DATA;
    return PARSE_PACKET_OK;
}

void BDSPV1ReceiverCore::reset(bool need_wait_delimiter) {
    if (_reader) {
        _reader->reset_read_state(need_wait_delimiter);
    }
    _fsm_state = PACKET_HEADER;
    _received_packet_data_bytes = 0;
    if (_packet_context.data_ptr) {
        _deallocate_packet_memory();
    }
}

void BDSPV1ReceiverCore::set_error_handler(receiver_error_handler_t error_handler, void *context) {
    _error_handler = error_handler;
    _error_handler_context = context;
}

void BDSPV1ReceiverCore::set_packet_handler(packet_handler_t packet_handler, void *context) {
    _packet_handler = packet_handler;
    _packet_handler_context = context;
}

void BDSPV1ReceiverCore::set_service_packet_handler(packet_handler_t service_packet_handler, void *context) {
    _service_packet_handler = service_packet_handler;
    _service_packet_handler_context = context;
}

bdsp_set_stream_reader_status_t BDSPV1ReceiverCore::set_stream_reader(streams::IStreamReader *reader_ptr) {
    if (not reader_ptr) {
        return STREAM_READER_NULL_POINTER_ERROR;
    }
    stream_data_handler_t callback = [](uint8_t byte, read_status_t read_state, void *context) {
        reinterpret_cast<BDSPV1ReceiverCore *>(context)->parse_packet_byte(byte, read_state);
    };
    reader_ptr->set_stream_data_handler(callback, this);
    if (not reader_ptr->get_ready_status()) {
        reader_ptr->set_stream_data_handler(nullptr, nullptr);
        return STREAM_READER_NOT_READY_ERROR;
    }
    _reader = reader_ptr;
    return SET_STREAM_READER_OK;
}
