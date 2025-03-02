#ifndef BDSP_STREAMS_PPP_WRITER_H
#define BDSP_STREAMS_PPP_WRITER_H

#include <stdint.h>

#include "BDSP/strategies/byte_stuffing/write.h"
#include "BDSP/streams/writer.h"

namespace BDSP::streams {

class PPPWriterStream final: public StreamWriter<strategies::PPPWriteStrategy> { };

} // namespace BDSP::streams

#endif // BDSP_STREAMS_PPP_WRITER_H
