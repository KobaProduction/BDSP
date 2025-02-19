#ifndef BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_MIXINS_H
#define BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_MIXINS_H

#include "BDSP/streams/strategies/byte_stuffing/types.h"

namespace BDSP::streams::strategies::byte_stuffing::core {

class ByteStuffingConfigsMixin {
protected:
    virtual set_byte_stuffing_config_status _check_config(byte_stuffing_config_t config);

public:
    virtual byte_stuffing_config_t get_default_config();
};

class PPPConfigsMixin: public virtual ByteStuffingConfigsMixin {
public:
    byte_stuffing_config_t get_default_config() override;
};

} // namespace BDSP::streams::strategies::byte_stuffing::core

#endif // BDSP_STREAMS_STRATEGIES_BYTE_STUFFING_MIXINS_H
