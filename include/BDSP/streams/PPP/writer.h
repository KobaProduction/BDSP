#ifndef BDSP_STREAMS_PPP_WRITER_H
#define BDSP_STREAMS_PPP_WRITER_H

#include <stdint.h>

#include "BDSP/streams/PPP/types.h"
#include "BDSP/streams/abstract/writer.h"

namespace BDSP::streams::PPP {

namespace core {
class PPPWriterCore: public ABS::AbstractStreamWriter {
protected:
    ppp_config_t _cfg;

    void _finish() override;
    void _process_byte(uint8_t byte) override;

public:
    ppp_config_t get_config();
    set_ppp_config_status set_config(ppp_config_t config);
};
} // namespace core

class PPPWriter final: public core::PPPWriterCore { };

} // namespace BDSP::streams::PPP

#endif // BDSP_STREAMS_PPP_WRITER_H
