#include <gtest/gtest.h>

#include "BDSP/streams/mixins.h"

using namespace BDSP::streams::core;

namespace {
class TestSimpleStream: public StreamReadyMixin {
public:
    using StreamReadyMixin::_state;
    using StreamReadyMixin::_set_handler_state;
    using StreamReadyMixin::_set_strategy_state;
};
} // namespace

TEST(streams_abstract_mixins_tests, stream_ready_mixin_states_test) {

    TestSimpleStream stream;

    stream._set_handler_state(false);
    stream._set_strategy_state(false);

    EXPECT_EQ(stream._state, StreamReadyMixin::NOT_READY);

    stream._set_handler_state(true);
    stream._set_strategy_state(false);

    EXPECT_EQ(stream._state, StreamReadyMixin::NOT_READY);

    stream._set_handler_state(false);
    stream._set_strategy_state(true);

    EXPECT_EQ(stream._state, StreamReadyMixin::NOT_READY);

    stream._set_handler_state(true);
    stream._set_strategy_state(true);

    EXPECT_EQ(stream._state, StreamReadyMixin::READY);
}
