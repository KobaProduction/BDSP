#include "BDSP/streams/COBS/mixins.h"

using namespace BDSP::streams::COBS;
using namespace BDSP::streams::COBS::core;

set_cobs_config_status COBSConfigCheckerMixin::_check_config(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced not_eq 0) {
        return ERROR_DEFAULT_COBS_SIZE_SR;
    }
    return SET_OK;
}

set_cobs_config_status COBSSRConfigCheckerMixin::_check_config(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced < 2) {
        return ERROR_SIZE_SR;
    }
    if (config.depth > 127) {
        return ERROR_DEPTH_SR;
    }
    return SET_OK;
}

set_cobs_config_status COBSZPEConfigCheckerMixin::_check_config(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced < 2) {
        return ERROR_SIZE_SR;
    }
    if (config.depth not_eq 224) {
        return ERROR_DEPTH_ZPE;
    }
    return SET_OK;
}
