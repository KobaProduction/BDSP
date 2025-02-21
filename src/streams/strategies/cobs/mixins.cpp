#include "BDSP/streams/strategies/COBS/mixins.h"

using namespace BDSP::streams::strategies::cobs;
using namespace BDSP::streams::strategies::cobs::core;

set_cobs_config_status COBSConfigsMixin::_check_config(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced not_eq 0) {
        return ERROR_DEFAULT_COBS_SIZE_SR;
    }
    return _check_min_cobs_depth(config);
}

set_cobs_config_status COBSConfigsMixin::_check_min_cobs_depth(cobs_config_t config) {
    if (config.depth < MIN_BDSP_COBS_DEPTH) {
        return ERROR_COBS_DEPTH;
    }
    return SET_OK;
}

cobs_config_t COBSConfigsMixin::get_default_config() {
    return {.delimiter = '\0',
            .depth = 255,
            .size_of_the_sequence_to_be_replaced = 0,
            .byte_of_the_sequence_to_be_replaced = '\0'};
}

set_cobs_config_status COBSSRConfigsMixin::_check_config(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced < 2) {
        return ERROR_SIZE_SR;
    }
    if (config.depth > 127) {
        return ERROR_DEPTH_SR;
    }
    return _check_min_cobs_depth(config);
}

cobs_config_t COBSSRConfigsMixin::get_default_config() {
    return {.delimiter = '\0',
            .depth = 127,
            .size_of_the_sequence_to_be_replaced = 2,
            .byte_of_the_sequence_to_be_replaced = '\0'};
}

set_cobs_config_status COBSZPEConfigsMixin::_check_config(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced < 2) {
        return ERROR_SIZE_SR;
    }
    if (config.depth not_eq 224) {
        return ERROR_DEPTH_ZPE;
    }
    return _check_min_cobs_depth(config);
}

cobs_config_t COBSZPEConfigsMixin::get_default_config() {
    return {.delimiter = '\0',
            .depth = 224,
            .size_of_the_sequence_to_be_replaced = 2,
            .byte_of_the_sequence_to_be_replaced = '\0'};
}
