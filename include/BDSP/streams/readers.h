#ifndef BDSP_STREAM_READERS_H
#define BDSP_STREAM_READERS_H

#include <stdint.h>

#include "BDSP/strategies/byte_stuffing/read.h"
#include "BDSP/strategies/cobs/read.h"
#include "BDSP/streams/reader.h"

namespace BDSP::streams {

class COBSReaderStream final: public StreamReader<strategies::COBSReadStrategy> { };
class COBSGSEReaderStream final: public StreamReader<strategies::COBSGSEReadStrategy> { };
class COBSZPEReaderStream final: public StreamReader<strategies::COBSZPEReadStrategy> { };

class PPPReaderStream final: public StreamReader<strategies::PPPReadStrategy> { };

} // namespace BDSP::streams

#endif // BDSP_STREAM_READERS_H
