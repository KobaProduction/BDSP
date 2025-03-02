#ifndef BDSP_STREAMS_PPP_READER_H
#define BDSP_STREAMS_PPP_READER_H

#include <stdint.h>

#include "BDSP/strategies/byte_stuffing/read.h"
#include "BDSP/streams/reader.h"

namespace BDSP::streams {

class PPPReaderStream final: public StreamReader<strategies::PPPReadStrategy> { };

} // namespace BDSP::streams

#endif // BDSP_STREAMS_PPP_READER_H
