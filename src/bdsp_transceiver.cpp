#include "bdsp_transceiver.h"

bdsp_set_config_status BDSPTransceiver::set_config(
        cobs_config_t config, write_handler_t write_handler_, packet_handler_t packet_handler_, void *context
) {
    bdsp_set_config_status status = BDSPTransmitter::set_config(config, write_handler_);
    if (status not_eq CONFIG_INSTALLED) return status;
    return BDSPReceiver::set_config(config, packet_handler_, context);
}
