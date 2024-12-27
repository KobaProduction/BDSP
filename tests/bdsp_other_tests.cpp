#include <gtest/gtest.h>

#include <BDSP/mixins.h>

using namespace BDSP::core;

TEST(bdsp_core_tests, max_packet_size_mixin_test) {
    MaxPacketSizeMixin mixin;
    auto status = mixin.set_max_packet_size(0);
    EXPECT_EQ(status, INVALID_MAXIMUM_PACKET_SIZE);

    status = mixin.set_max_packet_size(1);
    EXPECT_EQ(status, MAX_PACKET_SIZE_SET);
}
