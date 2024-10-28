#include "BDSP/BDSP.h"

#include <gtest/gtest.h>

TEST(add_test, add_1_1)
{
    EXPECT_EQ(BDSP::add(1, 1), 2);
}
