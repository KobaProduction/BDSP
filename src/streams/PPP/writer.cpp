#include "BDSP/streams/PPP/writer.h"
#include "BDSP/streams/PPP/checkers.h"

using namespace BDSP::streams::PPP;
using namespace BDSP::streams::PPP::core;

void PPPWriterCore::_process_byte(uint8_t byte) {
    if (byte not_eq _cfg.end_byte and byte not_eq _cfg.escape_byte)
        return _write(byte);
    _write(_cfg.escape_byte);
    _write(byte ^ _cfg.escape_mask);
}

void PPPWriterCore::_finish() {
    _write(_cfg.end_byte);
}

ppp_config_t PPPWriterCore::get_config() {
    return _cfg;
}

set_ppp_config_status PPPWriterCore::set_config(ppp_config_t config) {
    set_ppp_config_status status = check_ppp_config(config);
    if (status == SET_OK) {
        _cfg = config;
    }
    _set_ready_state(status == SET_OK);
    return status;
}
