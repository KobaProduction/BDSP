#ifndef BDSP_TRANSCEIVERS_H
#define BDSP_TRANSCEIVERS_H

#include "BDSP/streams/readers.h"
#include "BDSP/streams/writers.h"
#include "BDSP/transceiver.h"

namespace BDSP {

class BDSPTransceiverCOBS final: public BDSPTransceiver<streams::COBSReaderStream, streams::COBSWriterStream> { };
class BDSPTransceiverCOBSGSE final: public BDSPTransceiver<streams::COBSGSEReaderStream, streams::COBSGSEWriterStream> { };
class BDSPTransceiverCOBSZPE final: public BDSPTransceiver<streams::COBSZPEReaderStream, streams::COBSZPEWriterStream> { };

class BDSPTransceiverPPP final: public BDSPTransceiver<streams::PPPReaderStream, streams::PPPWriterStream> { };

} // namespace BDSP

#endif // BDSP_TRANSCEIVERS_H