#include <gtest/gtest.h>

#include "BDSP/strategies/cobs/write.h"

#include "testing.h"

using namespace BDSP::strategies::cobs;

namespace {
    const size_t constant_overhead = 2;

    template<typename TWriteStrategy>
    void redundancy_test(std::vector<uint8_t> &data, size_t encoded_bytes_size) {
        TWriteStrategy write_strategy;
        std::vector<uint8_t> encoded_data;

        write_strategy.init([] (uint8_t byte, void *ctx) {
            reinterpret_cast<std::vector<uint8_t> *>(ctx)->push_back(byte);
        }, nullptr, &encoded_data);
        for (uint8_t &byte: data) {
            write_strategy.write(byte);
        }
        write_strategy.finish();
        if (encoded_data.size() not_eq encoded_bytes_size) {
            show_data(data);
            show_data(encoded_data);
            EXPECT_EQ(encoded_data.size(), encoded_bytes_size);
        }
        data.clear();
    }
}

TEST(strategies_cobs_redundancy_tests, cobs_default_test) {
    std::vector<uint8_t> data;
    data.resize(1, 0x00);
    redundancy_test<COBSWriteStrategy>(data, 3);
    data.resize(1, 0x09);
    redundancy_test<COBSWriteStrategy>(data, 3);

    data.resize(UINT16_MAX, 0x00);
    redundancy_test<COBSWriteStrategy>(data, 65537);

    data.resize(253, 0x09);
    redundancy_test<COBSWriteStrategy>(data, 255);

    data.resize(254, 0x09);
    redundancy_test<COBSWriteStrategy>(data, 257);

    data.resize(255, 0x09);
    redundancy_test<COBSWriteStrategy>(data, 258);

    data.resize(65531, 0x09);
    redundancy_test<COBSWriteStrategy>(data, 65790);

    data.resize(65532, 0x09);
    redundancy_test<COBSWriteStrategy>(data, 65792);
}

TEST(strategies_cobs_redundancy_tests, cobs_gse_test) {
    std::vector<uint8_t> data;
    data.resize(1, 0x00);
    redundancy_test<COBSGSEWriteStrategy>(data, 3);
    data.resize(1, 0x09);
    redundancy_test<COBSGSEWriteStrategy>(data, 3);

    data.resize(65534, 0x00);
    redundancy_test<COBSGSEWriteStrategy>(data, 32769);

    data.resize(65535, 0x00);
    redundancy_test<COBSGSEWriteStrategy>(data, 32770);

    data.resize(125, 0x09);
    redundancy_test<COBSGSEWriteStrategy>(data, 127);

    data.resize(126, 0x09);
    redundancy_test<COBSGSEWriteStrategy>(data, 129);

    data.resize(65519, 0x09);
    redundancy_test<COBSGSEWriteStrategy>(data, 66040);

    data.resize(65520, 0x09);
    redundancy_test<COBSGSEWriteStrategy>(data, 66042);
}

TEST(strategies_cobs_redundancy_tests, cobs_zpe_test) {
    std::vector<uint8_t> data;
    data.resize(1, 0x00);
    redundancy_test<COBSZPEWriteStrategy>(data, 3);
    data.resize(1, 0x09);
    redundancy_test<COBSZPEWriteStrategy>(data, 3);

    data.resize(65534, 0x00);
    redundancy_test<COBSZPEWriteStrategy>(data, 32769);

    data.resize(65535, 0x00);
    redundancy_test<COBSZPEWriteStrategy>(data, 32770);

    data.resize(222, 0x09);
    redundancy_test<COBSZPEWriteStrategy>(data, 224);

    data.resize(223, 0x09);
    redundancy_test<COBSZPEWriteStrategy>(data, 226);

    data.resize(65338, 0x09);
    redundancy_test<COBSZPEWriteStrategy>(data, 65632);

    data.resize(65339, 0x09);
    redundancy_test<COBSZPEWriteStrategy>(data, 65634);
}