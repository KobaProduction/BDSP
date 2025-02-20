#ifndef BDSP_STREAMS_PPP_WRITER_H
#define BDSP_STREAMS_PPP_WRITER_H

#include <stdint.h>

#include "BDSP/streams/strategies/byte_stuffing/write.h"
#include "BDSP/streams/writer.h"

namespace BDSP::streams::ppp {

class PPPWriterStream final: public StreamWriter<strategies::byte_stuffing::PPPStreamWriteStrategy> { };

} // namespace BDSP::streams::ppp

#endif // BDSP_STREAMS_PPP_WRITER_H
