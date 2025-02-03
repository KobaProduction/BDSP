#include <gtest/gtest.h>

#include "BDSP/checksums/crc8.h"
#include "BDSP/mixins.h"

using namespace BDSP;
using namespace BDSP::core;
using namespace BDSP::checksums;

TEST(bdsp_core_tests, max_packet_size_mixin_test) {
    MaxPacketSizeMixin mixin;
    auto status = mixin.set_max_packet_size(0);
    EXPECT_EQ(status, INVALID_MAXIMUM_PACKET_SIZE);

    status = mixin.set_max_packet_size(1);
    EXPECT_EQ(status, MAX_PACKET_SIZE_SET);
}

TEST(bdsp_core_tests, bdsp_v1_checksum_mixin_test) {
    class ChecksumTester: public BDSPV1ChecksumMixin {
    public:
        uint8_t calc(packet_v1_header header, uint8_t *data_ptr, uint16_t size) {
            return _calc_checksum(header, data_ptr, size);
        }
    };

    ChecksumTester tester;

    EXPECT_EQ(tester.set_checksum_function(nullptr), CHECKSUM_FUNCTION_NULL_POINTER_ERROR);
    EXPECT_EQ(tester.set_checksum_function(crc8_nrsc5), SET_CHECKSUM_FUNCTION_OK);

    packet_v1_header header{};

    header.is_unsupported_protocol_version = false;
    header.is_two_bytes_for_packet_size = false;
    header.is_checksum_used = true;
    header.is_service_packet = false;
    header.packet_id = 1;


    size_t data_size = 1;
    auto *data = new uint8_t[data_size];
    for (int i = 0; i < data_size; ++i) data[i] = 0xAA;

    uint8_t correct_checksum = crc8_nrsc5(reinterpret_cast<uint8_t*>(&header), 1);
    uint8_t buffer = data_size;
    correct_checksum = crc8_nrsc5(&buffer, 1, correct_checksum);
    correct_checksum = crc8_nrsc5(data, data_size, correct_checksum);


    EXPECT_EQ(tester.calc(header, data, data_size), correct_checksum);
    delete [] data;

    data_size = 300;
    data = new uint8_t[data_size];
    for (int i = 0; i < data_size; ++i) data[i] = 0xAA;
    header.is_two_bytes_for_packet_size = true;

    correct_checksum = crc8_nrsc5(reinterpret_cast<uint8_t*>(&header), 1);
    buffer = data_size;
    correct_checksum = crc8_nrsc5(&buffer, 1, correct_checksum);
    buffer = data_size >> 8;
    correct_checksum = crc8_nrsc5(&buffer, 1, correct_checksum);
    correct_checksum = crc8_nrsc5(data, data_size, correct_checksum);
    EXPECT_EQ(tester.calc(header, data, data_size), correct_checksum);
    delete [] data;

    header.is_checksum_used = false;
    EXPECT_EQ(tester.calc(header, data, 1), 0);
}
