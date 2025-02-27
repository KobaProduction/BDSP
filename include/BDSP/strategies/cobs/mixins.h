#ifndef BDSP_STREAMS_STRATEGIES_COBS_MIXINS_H
#define BDSP_STREAMS_STRATEGIES_COBS_MIXINS_H

#include "BDSP/strategies/cobs/types.h"

namespace BDSP::strategies::cobs::core {

class COBSConfigsMixin {
protected:
    virtual set_cobs_config_status _check_config(cobs_config_t config);
    static set_cobs_config_status _check_min_cobs_depth(cobs_config_t config);

public:
    virtual cobs_config_t get_default_config();
};

class COBSSRConfigsMixin: public virtual COBSConfigsMixin {
protected:
    set_cobs_config_status _check_config(cobs_config_t config) override;

public:
    cobs_config_t get_default_config() override;
};

class COBSZPEConfigsMixin: public virtual COBSSRConfigsMixin {
protected:
    set_cobs_config_status _check_config(cobs_config_t config) override;

public:
    cobs_config_t get_default_config() override;
};

} // namespace BDSP::strategies::cobs::core

#endif // BDSP_STREAMS_STRATEGIES_COBS_MIXINS_H
