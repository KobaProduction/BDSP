#include <gtest/gtest.h>

#include "BDSP/streams/PPP/mixins.h"

using namespace BDSP::streams::PPP;
using namespace BDSP::streams::PPP::core;

TEST(ppp_mixins_tests, set_config_checker_test) {
    class TestPPPConfigChecker: public PPPConfigChecker {
    public:
        using PPPConfigChecker::_check_config;
    } checker;

    ppp_config_t config = {0x7E, 0x7D, 0x20};
    EXPECT_EQ(checker._check_config(config), SET_OK);

    config = {0x7E, 0x7D, 0x00};
    EXPECT_EQ(checker._check_config(config), ERROR_ESCAPE_MASK_IS_NULL);

    config = {0x7E, 0x7E, 0x20};
    EXPECT_EQ(checker._check_config(config), ERROR_END_AND_ESCAPE_BYTES_IS_EQUALS);

    config = {0x7E, 0x5E, 0x20};
    EXPECT_EQ(checker._check_config(config), ERROR_INVERTED_END_AND_ESCAPE_BYTES_IS_EQUALS);
}