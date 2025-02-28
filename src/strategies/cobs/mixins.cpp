#include "BDSP/strategies/cobs/mixins.h"

using namespace BDSP::strategies::cobs;
using namespace BDSP::strategies::cobs::core;

set_cobs_config_status COBSConfigsMixin::_check_config(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced not_eq 0) {
        return ERROR_DEFAULT_COBS_SIZE_GSE;
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
    cobs_config_t config;
    config.delimiter_byte = '\0';
    config.depth = 255;
    config.size_of_the_sequence_to_be_replaced = 0;
    config.byte_of_the_sequence_to_be_replaced = '\0';
    return config;
}

set_cobs_config_status COBSGSEConfigsMixin::_check_config(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced < 2) {
        return ERROR_SIZE_GSE;
    }
    if (config.depth > 127) {
        return ERROR_DEPTH_GSE;
    }
    return _check_min_cobs_depth(config);
}

cobs_config_t COBSGSEConfigsMixin::get_default_config() {
    cobs_config_t config;
    config.delimiter_byte = '\0';
    config.depth = 127;
    config.size_of_the_sequence_to_be_replaced = 2;
    config.byte_of_the_sequence_to_be_replaced = '\0';
    return config;
}

set_cobs_config_status COBSZPEConfigsMixin::_check_config(cobs_config_t config) {
    if (config.size_of_the_sequence_to_be_replaced < 2) {
        return ERROR_SIZE_GSE;
    }
    if (config.depth not_eq 224) {
        return ERROR_DEPTH_ZPE;
    }
    return _check_min_cobs_depth(config);
}

cobs_config_t COBSZPEConfigsMixin::get_default_config() {
    cobs_config_t config;
    config.delimiter_byte = '\0';
    config.depth = 224;
    config.size_of_the_sequence_to_be_replaced = 2;
    config.byte_of_the_sequence_to_be_replaced = '\0';
    return config;
}
