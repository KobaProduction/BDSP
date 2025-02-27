#ifndef BDSP_STREAMS_COBS_WRITER_H
#define BDSP_STREAMS_COBS_WRITER_H

#include "BDSP/strategies/cobs/write.h"
#include "BDSP/streams/writer.h"

namespace BDSP::streams::cobs {

class COBSWriterStream: public StreamWriter<strategies::cobs::COBSWriteStrategy> { };
class COBSSRWriterStream: public StreamWriter<strategies::cobs::COBSSRWriteStrategy> { };
class COBSZPEWriterStream: public StreamWriter<strategies::cobs::COBSZPEWriteStrategy> { };

} // namespace BDSP::streams::cobs

#endif // BDSP_STREAMS_COBS_WRITER_H
