#ifndef BDSP_TRANSCEIVER_H
#define BDSP_TRANSCEIVER_H

#include "BDSP/receiver.h"
#include "BDSP/transmitter.h"

namespace BDSP {
    class BDSPTransceiver : public BDSPTransmitter, public BDSPReceiver {
    };
}

#endif //BDSP_TRANSCEIVER_H