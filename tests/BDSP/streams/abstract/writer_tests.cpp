#include <gtest/gtest.h>
#include <vector>

#include "BDSP/streams/abstract/writer.h"

using namespace BDSP::streams;

namespace {
class TestAbstractStreamWriteStrategy final: public core::AbstractStreamWriteStrategy {
public:
    bool init_used = false;

    using AbstractStreamWriteStrategy::_ready_state_callback;
    using AbstractStreamWriteStrategy::_write_handler;
    using AbstractStreamWriteStrategy::_context;

    void _init() override { init_used = true; };
    void send_delimiter() override { write(0); }
    void write(uint8_t byte) override { _write_handler(byte, _context); }
};

struct ContextTestAbstractStreamWriteStrategy {
    bool ready_state = false;
    std::vector<uint8_t> bytes;
};

void write_handler(uint8_t byte, void *ctx) {
    reinterpret_cast<ContextTestAbstractStreamWriteStrategy *>(ctx)->bytes.push_back(byte);
};

void set_ready_state_callback(bool state, void *ctx) {
    reinterpret_cast<ContextTestAbstractStreamWriteStrategy *>(ctx)->ready_state = state;
};
} // namespace

TEST(abstract_writer_strategy_tests, init_test) {
    auto writer = TestAbstractStreamWriteStrategy();
    auto default_context_pointer = writer._context;
    auto default_write_handler = writer._write_handler;
    auto default_set_ready_state_callback = writer._ready_state_callback;

    writer.init(nullptr, nullptr, nullptr);
    ASSERT_TRUE(writer.init_used);
    EXPECT_EQ(default_context_pointer, writer._context);
    EXPECT_EQ(default_write_handler, writer._write_handler);
    EXPECT_EQ(default_set_ready_state_callback, writer._ready_state_callback);

    ContextTestAbstractStreamWriteStrategy context;

    context.ready_state = true;
    writer.init(nullptr, set_ready_state_callback, &context);
    EXPECT_EQ(&context, writer._context);
    EXPECT_EQ(default_write_handler, writer._write_handler);
    EXPECT_EQ(set_ready_state_callback, writer._ready_state_callback);
    ASSERT_FALSE(context.ready_state);

    context.ready_state = false;
    writer.init(write_handler, set_ready_state_callback, &context);
    EXPECT_EQ(&context, writer._context);
    EXPECT_EQ(write_handler, writer._write_handler);
    EXPECT_EQ(set_ready_state_callback, writer._ready_state_callback);
    ASSERT_TRUE(context.ready_state);
}

TEST(abstract_writer_strategy_tests, finish_test) {
    auto writer = TestAbstractStreamWriteStrategy();
    ContextTestAbstractStreamWriteStrategy context;
    writer.init(write_handler, set_ready_state_callback, &context);
    ASSERT_TRUE(context.ready_state);

    writer.finish();
    ASSERT_TRUE(context.bytes == std::vector<uint8_t>({0}));
}