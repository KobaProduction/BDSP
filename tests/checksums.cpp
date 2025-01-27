#include <string>
#include <cstdint>
#include <gtest/gtest.h>

#include "BDSP/checksums/crc8.h"

using namespace BDSP::checksums;


TEST(checksums_tests, crc8_segments_test) {
    std::string data = "Binary Data";
    uint8_t crc = core::crc8(reinterpret_cast<uint8_t *>(data.data()), data.size(), 0xFF, 0x31);
    data = " Separation Protocol";
    EXPECT_EQ(core::crc8(reinterpret_cast<uint8_t *>(data.data()), data.size(), crc, 0x31), 0x1B);
}

TEST(checksums_tests, crc8_cdma2000_test) {
    std::string data = "Binary Data Separation Protocol";
    EXPECT_EQ(crc8_cdma2000(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0x3A);
}

TEST(checksums_tests, crc8_dvb_s2_test) {
    std::string data = "Binary Data Separation Protocol";
    EXPECT_EQ(crc8_dvb_s2(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0x2D);
}

TEST(checksums_tests, crc8_gsm_a_test) {
    std::string data = "Binary Data Separation Protocol";
    EXPECT_EQ(crc8_gsm_a(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0x9A);
}

TEST(checksums_tests, crc8_hitag_test) {
    std::string data = "Binary Data Separation Protocol";
    EXPECT_EQ(crc8_hitag(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0x66);
}

TEST(checksums_tests, crc8_i_code_test) {
    std::string data = "Binary Data Separation Protocol";
    EXPECT_EQ(crc8_i_code(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0x50);
}

TEST(checksums_tests, crc8_lte_test) {
    std::string data = "Binary Data Separation Protocol";
    EXPECT_EQ(crc8_lte(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0xBE);
}

TEST(checksums_tests, crc8_mifare_mad_test) {
    std::string data = "Binary Data Separation Protocol";
    EXPECT_EQ(crc8_mifare_mad(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0x54);
}

TEST(checksums_tests, crc8_nrsc5_test) {
    std::string data = "Binary Data Separation Protocol";
    EXPECT_EQ(crc8_nrsc5(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0x1B);
}

TEST(checksums_tests, crc8_opensafety_test) {
    std::string data = "Binary Data Separation Protocol";
    EXPECT_EQ(crc8_opensafety(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0xC0);
}

TEST(checksums_tests, crc8_smbus_test) {
    std::string data = "Binary Data Separation Protocol";
    EXPECT_EQ(crc8_smbus(reinterpret_cast<uint8_t *>(data.data()), data.size()), 0xF9);
}