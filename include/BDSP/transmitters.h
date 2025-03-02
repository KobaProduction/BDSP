#ifndef BDSP_TRANSMITTERS_H
#define BDSP_TRANSMITTERS_H

#include "BDSP/streams/writers.h"
#include "BDSP/transmitter.h"

namespace BDSP {

class BDSPTransmitterCOBS final: public BDSPTransmitter<streams::COBSWriterStream> { };
class BDSPTransmitterCOBSGSE final: public BDSPTransmitter<streams::COBSGSEWriterStream> { };
class BDSPTransmitterCOBSZPE final: public BDSPTransmitter<streams::COBSZPEWriterStream> { };

class BDSPTransmitterPPP final: public BDSPTransmitter<streams::PPPWriterStream> { };

} // namespace BDSP

#endif // BDSP_TRANSMITTERS_H