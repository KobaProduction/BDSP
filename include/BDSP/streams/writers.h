#ifndef BDSP_STREAM_WRITERS_H
#define BDSP_STREAM_WRITERS_H

#include "BDSP/strategies/byte_stuffing/write.h"
#include "BDSP/strategies/cobs/write.h"
#include "BDSP/streams/writer.h"

namespace BDSP::streams {

class COBSWriterStream: public StreamWriter<strategies::COBSWriteStrategy> { };
class COBSGSEWriterStream: public StreamWriter<strategies::COBSGSEWriteStrategy> { };
class COBSZPEWriterStream: public StreamWriter<strategies::COBSZPEWriteStrategy> { };

class PPPWriterStream final: public StreamWriter<strategies::PPPWriteStrategy> { };

} // namespace BDSP::streams

#endif // BDSP_STREAM_WRITERS_H
