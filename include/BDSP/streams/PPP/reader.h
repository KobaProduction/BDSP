#ifndef BDSP_STREAMS_PPP_READER_H
#define BDSP_STREAMS_PPP_READER_H

#include <stdint.h>

#include "BDSP/streams/reader.h"
#include "BDSP/streams/strategies/byte_stuffing/read.h"

namespace BDSP::streams::ppp {

class PPPReaderStream final: public StreamReader<streams::strategies::byte_stuffing::PPPReadStrategy> { };

} // namespace BDSP::streams::ppp

#endif // BDSP_STREAMS_PPP_READER_H
