#include <gtest/gtest.h>

#include "BDSP/streams/abstract/mixins.h"

using namespace BDSP::streams::core;

TEST(abstract_stream_tests, abstract_stream_states_test) {
    class SimpleStream: public StreamReadyMixin {
    public:
        using StreamReadyMixin::_state;
        using StreamReadyMixin::_set_handler_state;
        using StreamReadyMixin::_set_strategy_state;
    };

    SimpleStream stream;

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
