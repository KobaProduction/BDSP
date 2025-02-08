#ifndef BDSP_STREAMS_PPP_READER_H
#define BDSP_STREAMS_PPP_READER_H

#include <stdint.h>

#include "BDSP/streams/PPP/types.h"
#include "BDSP/streams/abstract/reader.h"

namespace BDSP::streams::PPP {

namespace core {
class PPPReaderCore: public ABS::AbstractStreamReader {
protected:
    ppp_config_t _cfg;
    bool _is_escape_state = false;

    read_status_t _process_byte(uint8_t byte) override;
    void _reset() override;

public:
    ppp_config_t get_config();
    set_ppp_config_status set_config(ppp_config_t config);
};
} // namespace core

class PPPReader final: public core::PPPReaderCore { };

} // namespace BDSP::streams::PPP

#endif // BDSP_STREAMS_PPP_READER_H
