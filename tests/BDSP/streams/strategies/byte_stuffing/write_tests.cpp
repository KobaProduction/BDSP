#include <gtest/gtest.h>

#include "BDSP/streams/strategies/byte_stuffing/write.h"

using namespace BDSP::streams::strategies;
using namespace BDSP::streams::strategies::byte_stuffing;
using namespace BDSP::streams::strategies::byte_stuffing::core;

TEST(byte_stuffing_write_strategy_tests, set_config) {
    ByteStuffingWriteStrategyCore write_strategy;
    auto config = write_strategy.get_config();
    EXPECT_EQ(config.delimiter_byte, 0xFE);
    EXPECT_EQ(config.escape_byte, 0xFD);
    EXPECT_EQ(config.escape_mask, 0x20);

    bool ready_state = false;
    write_strategy.init(nullptr, [](bool state, void *ctx) { *reinterpret_cast<bool *>(ctx) = state; }, &ready_state);

    EXPECT_EQ(write_strategy.set_config(config), SET_OK);
    ASSERT_TRUE(ready_state);

    config.delimiter_byte = config.escape_byte;
    EXPECT_EQ(write_strategy.set_config(config), ERROR_END_AND_ESCAPE_BYTES_IS_EQUALS);
    ASSERT_FALSE(ready_state);
}

TEST(byte_stuffing_write_strategy_tests, write_test) {
    ByteStuffingWriteStrategyCore write_strategy;
    auto config = write_strategy.get_config();

    std::vector<uint8_t> stream;
    write_strategy.init([](uint8_t byte, void *ctx) { reinterpret_cast<std::vector<uint8_t> *>(ctx)->push_back(byte); },
                        nullptr,
                        &stream);

    write_strategy.write(0x00);
    ASSERT_TRUE(stream == std::vector<uint8_t>({0x00}));

    stream.clear();
    write_strategy.write(config.delimiter_byte);
    uint8_t escaped_byte = config.delimiter_byte ^ config.escape_mask;
    ASSERT_TRUE(stream == std::vector<uint8_t>({config.escape_byte, escaped_byte}));

    stream.clear();
    write_strategy.write(config.escape_byte);
    escaped_byte = config.escape_byte ^ config.escape_mask;
    ASSERT_TRUE(stream == std::vector<uint8_t>({config.escape_byte, escaped_byte}));

    stream.clear();
    write_strategy.send_delimiter();
    ASSERT_TRUE(stream == std::vector<uint8_t>({config.delimiter_byte}));
}