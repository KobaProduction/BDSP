#include <gtest/gtest.h>

#include "BDSP/streams/strategies/byte_stuffing/read.h"

using namespace BDSP::streams::strategies;
using namespace BDSP::streams::strategies::byte_stuffing;
using namespace BDSP::streams::strategies::byte_stuffing::core;

namespace {
struct Context {
    std::vector<uint8_t> data;
    std::vector<strategy_read_status_t> statuses;
};

void read_handler(uint8_t byte, strategy_read_status_t status, void *ctx) {
    reinterpret_cast<Context *>(ctx)->data.push_back(byte);
    reinterpret_cast<Context *>(ctx)->statuses.push_back(status);
}

void reset(Context &ctx, ByteStuffingReadStrategyCore &strategy) {
    strategy.reset_read_state();
    ctx.data.clear();
    ctx.statuses.clear();
}
} // namespace

TEST(byte_stuffing_read_strategy_tests, set_config) {
    ByteStuffingReadStrategyCore read_strategy;
    auto config = read_strategy.get_config();
    EXPECT_EQ(config.delimiter_byte, 0xFE);
    EXPECT_EQ(config.escape_byte, 0xFD);
    EXPECT_EQ(config.escape_mask, 0x20);

    bool ready_state = false;
    read_strategy.init(nullptr, [] (bool state, void *ctx) {
        *reinterpret_cast<bool*>(ctx) = state;
    }, &ready_state);

    EXPECT_EQ(read_strategy.set_config(config), SET_OK);
    ASSERT_TRUE(ready_state);

    read_strategy.read(config.escape_byte);
    EXPECT_EQ(read_strategy.set_config(config), ERROR_PROCESS_NOT_FINISHED);
    ASSERT_FALSE(ready_state);
}

TEST(byte_stuffing_read_strategy_tests, error_read_tests) {
    ByteStuffingReadStrategyCore read_strategy;
    auto config = read_strategy.get_default_config();

    Context context;
    read_strategy.init(read_handler, nullptr, &context);

    EXPECT_EQ(read_strategy.read(config.escape_byte), STRATEGY_READ_OK);
    EXPECT_EQ(read_strategy.set_config(config), ERROR_PROCESS_NOT_FINISHED);
    ASSERT_TRUE(context.data.empty() and context.statuses.empty());

    reset(context, read_strategy);
    EXPECT_EQ(read_strategy.read(config.escape_byte), STRATEGY_READ_OK);
    EXPECT_EQ(read_strategy.read(config.delimiter_byte ^ config.escape_mask + 1), STRATEGY_READ_ERROR);
    ASSERT_TRUE(context.data == std::vector<uint8_t>({0xFF}));
    ASSERT_TRUE(context.statuses == std::vector<strategy_read_status_t>({STRATEGY_READ_ERROR}));

    reset(context, read_strategy);
    EXPECT_EQ(read_strategy.read(config.escape_byte), STRATEGY_READ_OK);
    EXPECT_EQ(read_strategy.read(config.delimiter_byte), STRATEGY_READ_DELIMITER);
    ASSERT_TRUE(context.data == std::vector<uint8_t>({config.delimiter_byte, config.delimiter_byte}));
    ASSERT_TRUE(context.statuses == std::vector<strategy_read_status_t>({STRATEGY_READ_ERROR, STRATEGY_READ_DELIMITER}));
}

TEST(byte_stuffing_read_strategy_tests, correct_read_tests) {
    ByteStuffingReadStrategyCore read_strategy;
    auto config = read_strategy.get_default_config();

    Context context;
    read_strategy.init(read_handler, nullptr, &context);

    EXPECT_EQ(read_strategy.read(0x00), STRATEGY_READ_OK);
    EXPECT_EQ(read_strategy.read(0x00), STRATEGY_READ_OK);
    ASSERT_TRUE(context.data == std::vector<uint8_t>({0x00, 0x00}));
    ASSERT_TRUE(context.statuses == std::vector<strategy_read_status_t>({STRATEGY_READ_OK, STRATEGY_READ_OK}));

    reset(context, read_strategy);
    EXPECT_EQ(read_strategy.read(config.delimiter_byte), STRATEGY_READ_DELIMITER);
    ASSERT_TRUE(context.data == std::vector<uint8_t>({config.delimiter_byte}));
    ASSERT_TRUE(context.statuses == std::vector<strategy_read_status_t>({STRATEGY_READ_DELIMITER}));

    reset(context, read_strategy);
    EXPECT_EQ(read_strategy.read(config.escape_byte), STRATEGY_READ_OK);
    EXPECT_EQ(read_strategy.read(config.escape_byte ^ config.escape_mask), STRATEGY_READ_OK);
    ASSERT_TRUE(context.data == std::vector<uint8_t>({config.escape_byte}));
    ASSERT_TRUE(context.statuses == std::vector<strategy_read_status_t>({STRATEGY_READ_OK}));

    reset(context, read_strategy);
    EXPECT_EQ(read_strategy.read(config.escape_byte), STRATEGY_READ_OK);
    EXPECT_EQ(read_strategy.read(config.delimiter_byte ^ config.escape_mask), STRATEGY_READ_OK);
    ASSERT_TRUE(context.data == std::vector<uint8_t>({config.delimiter_byte}));
    ASSERT_TRUE(context.statuses == std::vector<strategy_read_status_t>({STRATEGY_READ_OK}));
}