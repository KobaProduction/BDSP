#include <gtest/gtest.h>

#include "testing.h"
#include "show.h"

using namespace BDSP::streams;

bool is_equals(const uint8_t *data1, size_t size1, const uint8_t *data2, size_t size2) {
    if (size1 not_eq size2) return false;
    bool equals = true;
    for (size_t i = 0; i < size1; ++i) {
        if (data1[i] == data2[i]) continue;
        equals = false;
        break;
    }
    return equals;
}

bool is_equals(const uint8_t *data1, size_t size1, std::vector<uint8_t> data2) {
    return is_equals(data1, size1, data2.data(), data2.size());
}

bool is_equals(std::vector<uint8_t> data1, std::vector<uint8_t> data2) {
    return is_equals(data1.data(), data1.size(), data2.data(), data2.size());
}

void check_data_for_correctness(std::vector<uint8_t> &current, std::vector<uint8_t> &correct, bool use_hex_when_show) {
    if (is_equals(correct, current)) return;
    std::cout << "Correct ";
    show_data(correct, use_hex_when_show);
    std::cout << "Current ";
    show_data(current, use_hex_when_show);
    FAIL() << "the current and correct array is not equal";
}

void start_test_writer(
        BDSP::streams::IStreamWriter &writer,
        std::vector<uint8_t> &current,
        std::vector<uint8_t> &correct,
        bool use_hex_when_show
) {
    std::vector<uint8_t> write_buffer;

    writer.set_stream_writer([](uint8_t byte, void *ctx_ptr) {
        reinterpret_cast<std::vector<uint8_t> *>(ctx_ptr)->push_back(byte);
    }, &write_buffer);

    auto status = writer.write(current.data(), current.size());
    writer.finish();
    if (status not_eq BDSP::streams::STREAM_WRITE_OK) {
        show_status(status);
        FAIL() << "Incorrect state";
    }
    check_data_for_correctness(write_buffer, correct);
}

void start_test_reader(
        BDSP::streams::IStreamReader &reader,
        std::vector<uint8_t> &current,
        std::vector<uint8_t> &correct,
        bool use_hex_when_show
) {
    struct Context {
        std::vector<uint8_t> read_buffer;
        bool is_ended = false;
    } ctx;

    reader.set_stream_data_handler([](uint8_t byte, read_status_t status, void *ctx_ptr) {
        auto context = reinterpret_cast<Context *>(ctx_ptr);
        ASSERT_FALSE(context->is_ended);
        switch (status) {
            case STREAM_READ_OK:
                context->read_buffer.push_back(byte);
                break;
            case STREAM_READ_END:
                context->is_ended = true;
                break;
            default:
                std::cout << "Symbol: " << uint32_t(byte) << " - ";
                show_status(status);
                FAIL() << "Incorrect state";
        }

    }, &ctx);

    reader.read(current.data(), current.size());
    check_data_for_correctness(ctx.read_buffer, correct);
}
