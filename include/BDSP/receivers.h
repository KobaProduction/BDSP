#ifndef BDSP_RECEIVERS_H
#define BDSP_RECEIVERS_H

#include "BDSP/receiver.h"
#include "BDSP/streams/readers.h"

namespace BDSP {

class BDSPReceiverCOBS final: public BDSPReceiver<streams::COBSReaderStream> {};
class BDSPReceiverCOBSGSE final: public BDSPReceiver<streams::COBSGSEReaderStream> {};
class BDSPReceiverCOBSZPE final: public BDSPReceiver<streams::COBSZPEReaderStream> {};

class BDSPReceiverPPP final: public BDSPReceiver<streams::PPPReaderStream> {};

} // namespace BDSP

#endif // BDSP_RECEIVERS_H