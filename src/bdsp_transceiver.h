#ifndef BDSP_BDSP_TRANSCEIVER_H
#define BDSP_BDSP_TRANSCEIVER_H

#include "bdsp_receiver.h"
#include "bdsp_transmitter.h"


class BDSPTransceiver : public BDSPTransmitter, public BDSPReceiver {
public:
    bdsp_set_config_status set_config(
            cobs_config_t config, write_handler_t write_handler, packet_handler_t packet_handler
    );
};

#endif //BDSP_BDSP_TRANSCEIVER_H