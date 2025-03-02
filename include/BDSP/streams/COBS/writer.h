#ifndef BDSP_STREAMS_COBS_WRITER_H
#define BDSP_STREAMS_COBS_WRITER_H

#include "BDSP/strategies/cobs/write.h"
#include "BDSP/streams/writer.h"

namespace BDSP::streams {

class COBSWriterStream: public StreamWriter<strategies::COBSWriteStrategy> { };
class COBSGSEWriterStream: public StreamWriter<strategies::COBSGSEWriteStrategy> { };
class COBSZPEWriterStream: public StreamWriter<strategies::COBSZPEWriteStrategy> { };

} // namespace BDSP::streams

#endif // BDSP_STREAMS_COBS_WRITER_H
