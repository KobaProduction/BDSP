#include <gtest/gtest.h>

#include "BDSP/streams/strategies/cobs/mixins.h"

using namespace BDSP::streams::strategies::cobs;
using namespace BDSP::streams::strategies::cobs::core;

TEST(cobs_mixins_tests, cobs_default_config_test) {
    cobs_config_t config = COBSConfigsMixin().get_default_config();

    EXPECT_EQ(config.delimiter_byte, '\0');
    EXPECT_EQ(config.depth, 255);
    EXPECT_EQ(config.size_of_the_sequence_to_be_replaced, 0);
    EXPECT_EQ(config.byte_of_the_sequence_to_be_replaced, '\0');
}

TEST(cobs_mixins_tests,cobs_default_checker_mixin_test) {
    class TestCOBSConfigChecker final: public COBSConfigsMixin {
    public:
        using COBSConfigsMixin::_check_config;
        using COBSConfigsMixin::_check_min_cobs_depth;
    } checker;

    cobs_config_t config;
    config.delimiter_byte = '\0';
    config.depth = 255;
    config.size_of_the_sequence_to_be_replaced = 0;
    config.byte_of_the_sequence_to_be_replaced = '\0';

    EXPECT_EQ(checker._check_config(config), SET_OK);

    config.size_of_the_sequence_to_be_replaced = 2;

    EXPECT_EQ(checker._check_config(config), ERROR_DEFAULT_COBS_SIZE_SR);

    config.depth = 0;
    EXPECT_EQ(checker._check_min_cobs_depth(config), ERROR_COBS_DEPTH);
}

TEST(cobs_mixins_tests, cobs_sr_config_test) {
    cobs_config_t config = COBSSRConfigsMixin().get_default_config();

    EXPECT_EQ(config.delimiter_byte, '\0');
    EXPECT_EQ(config.depth, 127);
    EXPECT_EQ(config.size_of_the_sequence_to_be_replaced, 2);
    EXPECT_EQ(config.byte_of_the_sequence_to_be_replaced, '\0');
}

TEST(cobs_mixins_tests, cobs_sr_checker_mixin_test) {
    class TestCOBSSRConfigChecker final: public COBSSRConfigsMixin {
    public:
        using COBSSRConfigsMixin::_check_config;
    } checker;

    cobs_config_t config;
    config.delimiter_byte = '\0';
    config.depth = 127;
    config.size_of_the_sequence_to_be_replaced = 2;
    config.byte_of_the_sequence_to_be_replaced = '\0';

    EXPECT_EQ(checker._check_config(config), SET_OK);

    config.depth = 255;

    EXPECT_EQ(checker._check_config(config), ERROR_DEPTH_SR);

    config.size_of_the_sequence_to_be_replaced = 0;

    EXPECT_EQ(checker._check_config(config), ERROR_SIZE_SR);
}

TEST(cobs_mixins_tests, cobs_zpe_config_test) {
    cobs_config_t config = COBSZPEConfigsMixin().get_default_config();

    EXPECT_EQ(config.delimiter_byte, '\0');
    EXPECT_EQ(config.depth, 224);
    EXPECT_EQ(config.size_of_the_sequence_to_be_replaced, 2);
    EXPECT_EQ(config.byte_of_the_sequence_to_be_replaced, '\0');
}

TEST(cobs_mixins_tests, cobs_zpe_checker_mixin_test) {
    class TestCOBSZPEConfigChecker final: public COBSZPEConfigsMixin {
    public:
        using COBSZPEConfigsMixin::_check_config;
    } checker;

    cobs_config_t config;
    config.delimiter_byte = '\0';
    config.depth = 224;
    config.size_of_the_sequence_to_be_replaced = 2;
    config.byte_of_the_sequence_to_be_replaced = '\0';

    EXPECT_EQ(checker._check_config(config), SET_OK);

    config.depth = 255;

    EXPECT_EQ(checker._check_config(config), ERROR_DEPTH_ZPE);

    config.size_of_the_sequence_to_be_replaced = 0;

    EXPECT_EQ(checker._check_config(config), ERROR_SIZE_SR);
}