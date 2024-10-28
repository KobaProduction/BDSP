#include <gtest/gtest.h>

#include <string>
#include "BDSP/checksums/crc/crc8.h"

TEST(crc8_test, crc8_1_1) {
    std::string data = "123456789";
    EXPECT_EQ(crc8(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0xF7);
    data = "1234567890";
    EXPECT_EQ(crc8(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0xD0);
    data = "BDSP (Binary Data Separation Protocol)";
    EXPECT_EQ(crc8(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0x1E);

    data = "BDSP (Binary Data";
    uint8_t crc = crc8(reinterpret_cast<uint8_t *>(data.data()), data.size());
    data = " Separation Protocol)";
    EXPECT_EQ(crc8(reinterpret_cast<uint8_t *>(data.data()), data.size(), crc), 0x1E);
}
