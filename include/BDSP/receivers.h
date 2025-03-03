#ifndef BDSP_RECEIVERS_H
#define BDSP_RECEIVERS_H

#include "BDSP/receiver.h"
#include "BDSP/streams/readers.h"

namespace BDSP {

class COBSReceiver final: public Receiver<streams::COBSReaderStream> {};
class COBSGSEReceiver final: public Receiver<streams::COBSGSEReaderStream> {};
class COBSZPEReceiver final: public Receiver<streams::COBSZPEReaderStream> {};

class PPPReceiver final: public Receiver<streams::PPPReaderStream> {};

} // namespace BDSP

#endif // BDSP_RECEIVERS_H