#ifndef BDSP_STREAMS_COBS_MIXINS_H
#define BDSP_STREAMS_COBS_MIXINS_H

#include "BDSP/streams/COBS/types.h"

namespace BDSP::streams::COBS::core {

class COBSConfigCheckerMixin {
protected:
    virtual set_cobs_config_status _check_config(cobs_config_t config);
};

class COBSSRConfigCheckerMixin: public virtual COBSConfigCheckerMixin {
protected:
    set_cobs_config_status _check_config(cobs_config_t config) override;
};

class COBSZPEConfigCheckerMixin: public virtual COBSSRConfigCheckerMixin {
protected:
    set_cobs_config_status _check_config(cobs_config_t config) override;
};

} // namespace BDSP::streams::COBS::core

#endif // BDSP_STREAMS_COBS_MIXINS_H
