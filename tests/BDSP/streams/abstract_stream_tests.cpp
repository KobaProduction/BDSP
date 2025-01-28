#include <gtest/gtest.h>

#include "BDSP/streams/abstract/stream.h"

using namespace BDSP::streams::ABS;

TEST(abstract_stream_tests, abstract_stream_states_test) {
    class SimpleStream: public AbstractStreamReadyMixin {
    public:
        StreamReadyState get_state() { return _state; }

        void set_handler_state(bool state) { _set_handler_state(state); }

        void set_ready_state(bool state) { _set_ready_state(state); }
    };

    SimpleStream stream;

    stream.set_handler_state(false);
    stream.set_ready_state(false);

    EXPECT_EQ(stream.get_state(), NOT_READY);

    stream.set_handler_state(true);
    stream.set_ready_state(false);

    EXPECT_EQ(stream.get_state(), NOT_READY);

    stream.set_handler_state(false);
    stream.set_ready_state(true);

    EXPECT_EQ(stream.get_state(), NOT_READY);

    stream.set_handler_state(true);
    stream.set_ready_state(true);

    EXPECT_EQ(stream.get_state(), READY);
}
