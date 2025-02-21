#ifndef BDSP_STREAMS_COBS_WRITER_H
#define BDSP_STREAMS_COBS_WRITER_H

#include "BDSP/streams/strategies/COBS/write.h"
#include "BDSP/streams/writer.h"

namespace BDSP::streams::cobs {

class COBSWriterStream: public StreamWriter<streams::strategies::cobs::COBSWriteStrategy> { };
class COBSSRWriterStream: public StreamWriter<streams::strategies::cobs::COBSSRWriteStrategy> { };
class COBSZPEWriterStream: public StreamWriter<streams::strategies::cobs::COBSZPEWriteStrategy> { };

} // namespace BDSP::streams::cobs

#endif // BDSP_STREAMS_COBS_WRITER_H
