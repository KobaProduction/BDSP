#ifndef BDSP_TRANSMITTERS_H
#define BDSP_TRANSMITTERS_H

#include "BDSP/streams/writers.h"
#include "BDSP/transmitter.h"

namespace BDSP {

class COBSTransmitter final: public Transmitter<streams::COBSWriterStream> { };
class COBSGSETransmitter final: public Transmitter<streams::COBSGSEWriterStream> { };
class COBSZPETransmitter final: public Transmitter<streams::COBSZPEWriterStream> { };

class PPPTransmitter final: public Transmitter<streams::PPPWriterStream> { };

} // namespace BDSP

#endif // BDSP_TRANSMITTERS_H