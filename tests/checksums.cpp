#include <string>
#include <cstdint>
#include <gtest/gtest.h>

#include "BDSP/checksums/crc8.h"

using namespace BDSP::utils;

TEST(checksums_tests, crc8_nrsc5_full_test) {
    std::string data = "BDSP (Binary Data Separation Protocol)";
    EXPECT_EQ(crc8_nrsc5(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0x1E);
}

TEST(checksums_tests, crc8_nrsc5_segments_test) {
    std::string data = "BDSP (Binary Data";
    uint8_t crc = crc8_nrsc5(reinterpret_cast<uint8_t *>(data.data()), data.size());
    data = " Separation Protocol)";
    EXPECT_EQ(crc8_nrsc5(reinterpret_cast<uint8_t *>(data.data()), data.size(), crc), 0x1E);
}
