#include <gtest/gtest.h>

#include "BDSP/streams/COBS/mixins.h"

using namespace BDSP::streams::COBS;
using namespace BDSP::streams::COBS::core;

TEST(cobs_mixins_tests, cobs_config_checker_mixin_test) {
    class TestCOBSConfigChecker final: public COBSConfigCheckerMixin {
    public:
        using COBSConfigCheckerMixin::_check_config;
    } checker;

    cobs_config_t config = {.delimiter = '\0',
                            .depth = 255,
                            .size_of_the_sequence_to_be_replaced = 0,
                            .byte_of_the_sequence_to_be_replaced = '\0'};

    EXPECT_EQ(checker._check_config(config), SET_OK);

    config.size_of_the_sequence_to_be_replaced = 2;

    EXPECT_EQ(checker._check_config(config), ERROR_DEFAULT_COBS_SIZE_SR);
}

TEST(cobs_mixins_tests, cobs_sr_checker_mixin_test) {
    class TestCOBSSRConfigChecker final: public COBSSRConfigCheckerMixin {
    public:
        using COBSSRConfigCheckerMixin::_check_config;
    } checker;

    cobs_config_t config = {.delimiter = '\0',
                            .depth = 127,
                            .size_of_the_sequence_to_be_replaced = 2,
                            .byte_of_the_sequence_to_be_replaced = '\0'};

    EXPECT_EQ(checker._check_config(config), SET_OK);

    config.depth = 255;

    EXPECT_EQ(checker._check_config(config), ERROR_DEPTH_SR);

    config.size_of_the_sequence_to_be_replaced = 0;

    EXPECT_EQ(checker._check_config(config), ERROR_SIZE_SR);
}

TEST(cobs_mixins_tests, cobs_zpe_checker_mixin_test) {
    class TestCOBSZPEConfigChecker final : public COBSZPEConfigCheckerMixin {
    public:
        using COBSZPEConfigCheckerMixin::_check_config;
    } checker;

    cobs_config_t config = {.delimiter = '\0',
                            .depth = 224,
                            .size_of_the_sequence_to_be_replaced = 2,
                            .byte_of_the_sequence_to_be_replaced = '\0'};

    EXPECT_EQ(checker._check_config(config), SET_OK);

    config.depth = 255;

    EXPECT_EQ(checker._check_config(config), ERROR_DEPTH_ZPE);

    config.size_of_the_sequence_to_be_replaced = 0;

    EXPECT_EQ(checker._check_config(config), ERROR_SIZE_SR);
}