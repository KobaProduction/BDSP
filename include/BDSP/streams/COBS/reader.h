#ifndef BDSP_STREAMS_COBS_READER_H
#define BDSP_STREAMS_COBS_READER_H

#include "BDSP/strategies/cobs/read.h"
#include "BDSP/streams/reader.h"

namespace BDSP::streams {

class COBSReaderStream final: public StreamReader<strategies::COBSReadStrategy> { };
class COBSGSEReaderStream final: public StreamReader<strategies::COBSGSEReadStrategy> { };
class COBSZPEReaderStream final: public StreamReader<strategies::COBSZPEReadStrategy> { };

} // namespace BDSP::streams

#endif // BDSP_STREAMS_COBS_READER_H
