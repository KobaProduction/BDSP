#ifndef BDSP_TRANSCEIVER_H
#define BDSP_TRANSCEIVER_H

#include <BDSP/receiver.h>
#include <BDSP/transmitter.h>

namespace BDSP {
    class BDSPTransceiver : public BDSPTransmitter, public BDSPReceiver {
    public:
        set_config_status_t set_config(
            COBS::config_t cobs_config,
            COBS::write_handler_t write_handler,
            packet_handler_t packet_handler,
            void *write_handler_context_ptr = nullptr,
            void *packet_handler_context_ptr = nullptr
        );
    };
}


#endif //BDSP_TRANSCEIVER_H