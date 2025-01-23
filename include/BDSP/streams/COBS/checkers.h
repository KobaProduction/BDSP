#ifndef BDSP_CHECKERS_H
#define BDSP_CHECKERS_H

#include "BDSP/streams/COBS/types.h"

namespace BDSP::streams::COBS::core {

set_config_status cobs_default_config_checker(cobs_config_t config);
set_config_status cobs_sr_config_checker(cobs_config_t config);
set_config_status cobs_zpe_config_checker(cobs_config_t config);

} // namespace BDSP::streams::COBS::core

#endif // BDSP_CHECKERS_H
