#include <gtest/gtest.h>
#include <vector>

#include "BDSP/strategies/abstract/read.h"

using namespace BDSP::strategies;

namespace {
class TestAbstractStreamReadStrategy final: public abstract::AbstractReadStrategy {
public:
    bool init_used = false;

    using AbstractReadStrategy::_context;
    using AbstractReadStrategy::_data_callback;
    using AbstractReadStrategy::_ready_state_callback;

    void _init() override { init_used = true; };
    strategy_read_status_t read(uint8_t byte) override { return STRATEGY_READ_OK; }
    void reset_read_state() override { }
};
} // namespace

TEST(abstract_reader_strategy_tests, init_test) {
    struct Context {
        bool ready_state = false;
        std::vector<uint8_t> bytes;
        std::vector<strategy_read_status_t> statuses;
    };

    auto read_callback = [](uint8_t byte, strategy_read_status_t status, void *ctx) { };

    auto set_ready_state_callback = [](bool state, void *ctx) {
        reinterpret_cast<Context *>(ctx)->ready_state = state;
    };

    auto reader = TestAbstractStreamReadStrategy();
    auto default_context_pointer = reader._context;
    auto default_read_callback = reader._data_callback;
    auto default_set_ready_state_callback = reader._ready_state_callback;

    reader.init(nullptr, nullptr, nullptr);
    ASSERT_TRUE(reader.init_used);
    EXPECT_EQ(default_context_pointer, reader._context);
    EXPECT_EQ(default_read_callback, reader._data_callback);
    EXPECT_EQ(default_set_ready_state_callback, reader._ready_state_callback);

    Context context;

    context.ready_state = true;
    reader.init(nullptr, set_ready_state_callback, &context);
    EXPECT_EQ(&context, reader._context);
    EXPECT_EQ(default_read_callback, reader._data_callback);
    EXPECT_EQ(set_ready_state_callback, reader._ready_state_callback);
    ASSERT_FALSE(context.ready_state);

    context.ready_state = false;
    reader.init(read_callback, set_ready_state_callback, &context);
    EXPECT_EQ(&context, reader._context);
    EXPECT_EQ(read_callback, reader._data_callback);
    EXPECT_EQ(set_ready_state_callback, reader._ready_state_callback);
    ASSERT_TRUE(context.ready_state);
}
