#include <string>
#include <cstdint>
#include <gtest/gtest.h>

#include <BDSP/checksums/crc/crc8.h>

TEST(checksums_tests, crc8_full_test) {
    std::string data = "BDSP (Binary Data Separation Protocol)";
    EXPECT_EQ(crc8(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0x1E);
}

TEST(checksums_tests, crc8_segments_test) {
    std::string data = "BDSP (Binary Data";
    uint8_t crc = crc8(reinterpret_cast<uint8_t *>(data.data()), data.size());
    data = " Separation Protocol)";
    EXPECT_EQ(crc8(reinterpret_cast<uint8_t *>(data.data()), data.size(), crc), 0x1E);
}
