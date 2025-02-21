#ifndef BDSP_STREAMS_COBS_READER_H
#define BDSP_STREAMS_COBS_READER_H

#include "BDSP/streams/reader.h"
#include "BDSP/streams/strategies/COBS/read.h"

namespace BDSP::streams::cobs {

class COBSReaderStream final: public StreamReader<streams::strategies::cobs::COBSReadStrategy> { };
class COBSSRReaderStream final: public StreamReader<streams::strategies::cobs::COBSSRReadStrategy> { };
class COBSZPEReaderStream final: public StreamReader<streams::strategies::cobs::COBSZPEReadStrategy> { };

} // namespace BDSP::streams::cobs

#endif // BDSP_STREAMS_COBS_READER_H
