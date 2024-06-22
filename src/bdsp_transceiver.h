#ifndef BDSP_BDSP_TRANSCEIVER_H
#define BDSP_BDSP_TRANSCEIVER_H

#include "bdsp_receiver.h"
#include "bdsp_transmitter.h"


class BDSPTransceiver : public BDSPTransmitter, public BDSPReceiver {
public:
    BDSP::set_config_status_t set_config(
        COBS::config_t cobs_config,
        COBS::write_handler_t write_handler,
        BDSP::packet_handler_t packet_handler,
        void *write_handler_context_ptr = nullptr,
        void *packet_handler_context_ptr = nullptr
    );
};

#endif //BDSP_BDSP_TRANSCEIVER_H