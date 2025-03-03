#ifndef BDSP_TRANSCEIVERS_H
#define BDSP_TRANSCEIVERS_H

#include "BDSP/streams/readers.h"
#include "BDSP/streams/writers.h"
#include "BDSP/transceiver.h"

namespace BDSP {

class COBSTransceiver final: public Transceiver<streams::COBSReaderStream, streams::COBSWriterStream> { };
class COBSGSETransceiver final: public Transceiver<streams::COBSGSEReaderStream, streams::COBSGSEWriterStream> { };
class COBSZPETransceiver final: public Transceiver<streams::COBSZPEReaderStream, streams::COBSZPEWriterStream> { };

class PPPTransceiver final: public Transceiver<streams::PPPReaderStream, streams::PPPWriterStream> { };

} // namespace BDSP

#endif // BDSP_TRANSCEIVERS_H