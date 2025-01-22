#include "BDSP/streams/COBS/checkers.h"

namespace BDSP::streams::COBS::core {

bool cobs_default_config_checker(cobs_config_t &config, set_config_status &status) {
    if (config.size_of_the_sequence_to_be_replaced not_eq 0) {
        config.size_of_the_sequence_to_be_replaced = 0;
        status = WARNING_COBS_SIZE_SR;
    }
    return true;
}

bool cobs_sr_config_checker(cobs_config_t &config, set_config_status &status) {
    if (config.size_of_the_sequence_to_be_replaced < 2) {
        status = ERROR_SIZE_SR;
        return false;
    }

    if (config.depth > 127) {
        status = WARNING_DEPTH_SR;
        config.depth = 127;
    }

    return true;
}

bool cobs_zpe_config_checker(cobs_config_t &config, set_config_status &status) {
    if (config.size_of_the_sequence_to_be_replaced < 2) {
        status = ERROR_SIZE_SR;
        return false;
    }

    if (config.depth not_eq 224) {
        config.depth = 244;
        status = WARNING_DEPTH_ZPE;
    }
    return true;
}

} // namespace BDSP::streams::COBS::core
