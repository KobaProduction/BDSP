#include "BDSP/bdsp_transceiver.h"

BDSP::set_config_status_t BDSPTransceiver::set_config(
        COBS::config_t cobs_config,
        COBS::write_handler_t write_handler_,
        BDSP::packet_handler_t packet_handler_,
        void *write_handler_context_ptr,
        void *packet_handler_context_ptr
) {
    BDSP::set_config_status_t status = BDSPTransmitter::set_config(cobs_config, write_handler_, write_handler_context_ptr);
    if (status not_eq BDSP::CONFIG_INSTALLED) return status;
    return BDSPReceiver::set_config(cobs_config, packet_handler_, packet_handler_context_ptr);
}
