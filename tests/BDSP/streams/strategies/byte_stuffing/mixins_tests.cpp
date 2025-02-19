#include <gtest/gtest.h>

#include "BDSP/streams/strategies/byte_stuffing/mixins.h"

using namespace BDSP::streams::strategies::byte_stuffing;
using namespace BDSP::streams::strategies::byte_stuffing::core;

TEST(byte_stuffing_mixins_tests, get_default_config_test) {
    byte_stuffing_config_t config = ByteStuffingConfigsMixin().get_default_config();
    EXPECT_EQ(config.delimiter_byte, 0xFE);
    EXPECT_EQ(config.escape_byte, 0xFD);
    EXPECT_EQ(config.escape_mask, 0x20);
}

TEST(byte_stuffing_mixins_tests, get_ppp_config_test) {
    byte_stuffing_config_t config = PPPConfigsMixin().get_default_config();
    EXPECT_EQ(config.delimiter_byte, 0x7E);
    EXPECT_EQ(config.escape_byte, 0x7D);
    EXPECT_EQ(config.escape_mask, 0x20);
}

TEST(ppp_mixins_tests, set_config_checker_test) {
    class TestStuffingConfigsChecker: public ByteStuffingConfigsMixin {
    public:
        using ByteStuffingConfigsMixin::_check_config;
    } checker;

    byte_stuffing_config_t config = checker.get_default_config();
    EXPECT_EQ(checker._check_config(config), SET_OK);

    config = {0x7E, 0x7D, 0x00};
    EXPECT_EQ(checker._check_config(config), ERROR_ESCAPE_MASK_IS_NULL);

    config = {0x7E, 0x7E, 0x20};
    EXPECT_EQ(checker._check_config(config), ERROR_END_AND_ESCAPE_BYTES_IS_EQUALS);

    config = {0x7E, 0x5E, 0x20};
    EXPECT_EQ(checker._check_config(config), ERROR_INVERTED_END_AND_ESCAPE_BYTES_IS_EQUALS);
}