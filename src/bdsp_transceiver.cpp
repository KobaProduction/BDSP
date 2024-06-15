#include "bdsp_transceiver.h"

bdsp_set_config_status_t BDSPTransceiver::set_config(
        cobs_config_t cobs_config,
        cobs_write_handler_t write_handler_,
        packet_handler_t packet_handler_,
        void *write_handler_context_ptr,
        void *packet_handler_context_ptr
) {
    bdsp_set_config_status_t status = BDSPTransmitter::set_config(cobs_config, write_handler_, write_handler_context_ptr);
    if (status not_eq CONFIG_INSTALLED) return status;
    return BDSPReceiver::set_config(cobs_config, packet_handler_, packet_handler_context_ptr);
}
