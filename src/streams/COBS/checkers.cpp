#include "BDSP/streams/COBS/checkers.h"

using namespace BDSP::streams::COBS;

set_cobs_config_status core::cobs_default_config_checker(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced not_eq 0) {
        return ERROR_DEFAULT_COBS_SIZE_SR;
    }
    return SET_OK;
}

set_cobs_config_status core::cobs_sr_config_checker(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced < 2) {
        return ERROR_SIZE_SR;
    }

    if (config.depth > 127) {
        return ERROR_DEPTH_SR;
    }

    return SET_OK;
}

set_cobs_config_status core::cobs_zpe_config_checker(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced < 2) {
        return ERROR_SIZE_SR;
    }

    if (config.depth not_eq 224) {
        return ERROR_DEPTH_ZPE;
    }
    return SET_OK;
}