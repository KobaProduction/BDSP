#ifndef BDSP_STREAMS_COBS_READER_H
#define BDSP_STREAMS_COBS_READER_H

#include "BDSP/strategies/cobs/read.h"
#include "BDSP/streams/reader.h"

namespace BDSP::streams::cobs {

class COBSReaderStream final: public StreamReader<strategies::cobs::COBSReadStrategy> { };
class COBSGSEReaderStream final: public StreamReader<strategies::cobs::COBSGSEReadStrategy> { };
class COBSZPEReaderStream final: public StreamReader<strategies::cobs::COBSZPEReadStrategy> { };

} // namespace BDSP::streams::cobs

#endif // BDSP_STREAMS_COBS_READER_H
