#include "BDSP/streams/PPP/reader.h"
#include "BDSP/streams/PPP/checkers.h"

using namespace BDSP::streams;
using namespace BDSP::streams::PPP;
using namespace BDSP::streams::PPP::core;

void PPPReaderCore::_reset() {
    _is_escape_state = false;
}

read_status_t PPPReaderCore::_process_byte(uint8_t byte) {
    read_status_t status = STREAM_READ_OK;

    if (byte == _cfg.end_byte) {
        return _is_escape_state ? STREAM_READ_ERROR : STREAM_READ_END;
    }

    if (_is_escape_state) {
        _is_escape_state = false;
        byte ^= _cfg.escape_mask;
        if (byte not_eq _cfg.escape_byte and byte not_eq _cfg.end_byte) {
            return STREAM_READ_ERROR;
        }
    } else if (byte == _cfg.escape_byte) {
        _is_escape_state = true;
        return STREAM_READ_OK;
    }
    _handler(byte, status);
    return status;
}

ppp_config_t PPPReaderCore::get_config() {
    return _cfg;
}

set_ppp_config_status PPPReaderCore::set_config(ppp_config_t config) {
    set_ppp_config_status status = check_ppp_config(config);
    if (_is_escape_state) {
        status = ERROR_PROCESS_NOT_FINISHED;
    }
    if (status == SET_OK) {
        _cfg = config;
    }
    _set_ready_state(status == SET_OK);
    return status;
}
