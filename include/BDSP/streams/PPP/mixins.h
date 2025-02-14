#ifndef BDSP_STREAMS_PPP_MIXINS_H
#define BDSP_STREAMS_PPP_MIXINS_H

#include "BDSP/streams/PPP/types.h"

namespace BDSP::streams::PPP::core {

class PPPConfigChecker {
protected:
    virtual set_ppp_config_status _check_config(ppp_config_t config);
};

} // namespace BDSP::streams::PPP::core

#endif // BDSP_STREAMS_PPP_MIXINS_H
