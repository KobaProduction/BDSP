#ifndef BDSP_STREAMS_COBS_CHECKERS_H
#define BDSP_STREAMS_COBS_CHECKERS_H

#include "BDSP/streams/COBS/types.h"

namespace BDSP::streams::COBS::core {

set_cobs_config_status cobs_default_config_checker(cobs_config_t config);
set_cobs_config_status cobs_sr_config_checker(cobs_config_t config);
set_cobs_config_status cobs_zpe_config_checker(cobs_config_t config);

} // namespace BDSP::streams::COBS::core

#endif // BDSP_STREAMS_COBS_CHECKERS_H
