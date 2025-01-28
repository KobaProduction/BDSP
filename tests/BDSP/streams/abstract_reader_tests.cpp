#include <gtest/gtest.h>

#include "../../utils/testing.h"
#include "BDSP/streams/abstract/reader.h"

using namespace BDSP::streams;

class SimpleReader: public ABS::AbstractReader {
    read_status_t _process_byte(uint8_t byte) override {
        read_status_t status = STREAM_READ_OK;
        if (byte == error_byte)
            return STREAM_READ_ERROR;
        if (byte == end_byte)
            return STREAM_READ_END;
        _handler(byte, status);
        byte_counter++;
        return status;
    }

    void _reset() override { byte_counter = 0; };

public:
    uint8_t error_byte = 0xF0;
    uint8_t end_byte = 0xFF;

    size_t byte_counter = 0;
};

TEST(abstract_reader_tests, abstract_reader_errors_test) {
    auto reader = SimpleReader();
    auto status = reader.read(0x00);
    EXPECT_EQ(status, ERROR_READ_STREAM_NOT_READY);

    status = reader.read(&reader.error_byte, 1);
    EXPECT_EQ(status, ERROR_READ_STREAM_NOT_READY);

    reader.set_stream_data_handler([](uint8_t byte, read_status_t status, void *ctx) { EXPECT_EQ(status, STREAM_READ_ERROR); },
                                   nullptr);

    status = reader.read(reader.error_byte);
    EXPECT_EQ(status, STREAM_READ_ERROR);

    status = reader.read(&reader.error_byte, 1);
    EXPECT_EQ(status, STREAM_READ_ERROR);
}

TEST(abstract_reader_tests, abstract_reader_normal_using_test) {
    auto reader = SimpleReader();

    EXPECT_EQ(reader.get_ready_status(), false);

    auto normal_handler = [](uint8_t byte, read_status_t status, void *ctx) {
        EXPECT_EQ(byte, 0x00);
        EXPECT_EQ(status, STREAM_READ_OK);
        *reinterpret_cast<bool *>(ctx) = true;
    };

    bool handler_used = false;
    reader.set_stream_data_handler(normal_handler, &handler_used);
    EXPECT_EQ(reader.get_ready_status(), true);

    auto status = reader.read(0x00);
    EXPECT_EQ(status, STREAM_READ_OK);
    EXPECT_EQ(handler_used, true);

    EXPECT_EQ(reader.byte_counter, 1);
    reader.reset_read_state(false);
    EXPECT_EQ(reader.byte_counter, 0);

    std::vector<uint8_t> data = {0x00, 0x00, 0x00};
    status = reader.read(data.data(), data.size());
    EXPECT_EQ(status, STREAM_READ_OK);
    EXPECT_EQ(reader.byte_counter, data.size());

    handler_used = false;
    reader.set_stream_data_handler(
        [](uint8_t byte, read_status_t status, void *ctx) {
            *reinterpret_cast<bool *>(ctx) = true;
            EXPECT_EQ(status, STREAM_READ_END);
        },
        &handler_used);

    status = reader.read(reader.end_byte);
    EXPECT_EQ(handler_used, true);
    EXPECT_EQ(status, STREAM_READ_END);
    EXPECT_EQ(reader.byte_counter, 0);

    handler_used = false;
    reader.set_stream_data_handler(normal_handler, &handler_used);
    status = reader.read(0x00);
    EXPECT_EQ(handler_used, true);
    EXPECT_EQ(status, STREAM_READ_OK);
}

TEST(abstract_reader_tests, abstract_reader_read_correct_test) {
    struct Context {
        SimpleReader reader;
        std::vector<uint8_t> read_buffer;
        std::vector<uint8_t> correct = {0x00, 0x00};
        std::vector<uint8_t> write_buffer = correct;
        int number_of_end_reads = 0;
    } ctx;

    ctx.write_buffer.push_back(ctx.reader.end_byte);

    ctx.reader.set_stream_data_handler(
        [](uint8_t byte, read_status_t status, void *ctx) {
            reinterpret_cast<std::vector<uint8_t> *>(ctx)->push_back(byte);
        },
        &ctx.read_buffer);

    start_test_reader(ctx.reader, ctx.write_buffer, ctx.correct);
    EXPECT_EQ(ctx.reader.byte_counter, 0);

    ctx.reader.set_stream_data_handler(
        [](uint8_t byte, read_status_t status, void *ctx) {
            auto &context = *reinterpret_cast<Context *>(ctx);
            if (status == STREAM_READ_OK) {
                context.read_buffer.push_back(byte);
            } else if (status == STREAM_READ_END) {
                context.number_of_end_reads++;
                EXPECT_EQ(byte, context.reader.end_byte);
                EXPECT_EQ(is_equals(context.read_buffer, context.correct), true);
                context.read_buffer.clear();
            } else {
                FAIL() << "An implicit error has occurred.";
            }
        },
        &ctx);

    ctx.read_buffer.clear();
    ctx.write_buffer.insert(ctx.write_buffer.end(), ctx.correct.begin(), ctx.correct.end());
    ctx.write_buffer.push_back(ctx.reader.end_byte);
    auto status = ctx.reader.read(ctx.write_buffer.data(), ctx.write_buffer.size());
    EXPECT_EQ(status, STREAM_READ_OK);
    EXPECT_EQ(ctx.number_of_end_reads, 2);
}

TEST(abstract_reader_tests, abstract_reader_read_errors_test) {
    struct Context {
        SimpleReader reader;
        std::vector<uint8_t> read_buffer;
        std::vector<uint8_t> correct = {0x00, 0x00};
        std::vector<uint8_t> write_buffer = correct;

        int number_of_end_reads = 0;
        int number_of_error_reads = 0;
    } ctx;

    ctx.write_buffer.push_back(ctx.reader.error_byte);

    ctx.reader.set_stream_data_handler(
        [](uint8_t byte, read_status_t status, void *ctx) {
            auto &context = *reinterpret_cast<Context *>(ctx);
            if (status == STREAM_READ_OK) {
                context.read_buffer.push_back(byte);
            } else if (status == STREAM_READ_END) {
                context.number_of_end_reads++;
                EXPECT_EQ(byte, context.reader.end_byte);
            } else if (status == STREAM_READ_ERROR) {
                EXPECT_EQ(context.number_of_error_reads, 0);
                context.number_of_error_reads++;
                EXPECT_EQ(context.reader.byte_counter, 2);
                EXPECT_EQ(byte, context.reader.error_byte);
                context.read_buffer.clear();
            } else {
                FAIL() << "An implicit error has occurred.";
            }
        },
        &ctx);

    auto status = ctx.reader.read(ctx.write_buffer.data(), ctx.write_buffer.size());
    EXPECT_EQ(status, STREAM_READ_ERROR);

    EXPECT_EQ(ctx.read_buffer.size(), 0);

    auto new_correct = ctx.correct;
    new_correct.insert(new_correct.end(), ctx.correct.begin(), ctx.correct.end());

    ctx.write_buffer.push_back(ctx.reader.end_byte);
    ctx.write_buffer.insert(ctx.write_buffer.end(), new_correct.begin(), new_correct.end());
    ctx.write_buffer.push_back(ctx.reader.end_byte);

    ctx.number_of_error_reads = 0;
    status = ctx.reader.read(ctx.write_buffer.data(), ctx.write_buffer.size());
    EXPECT_EQ(status, STREAM_READ_ERROR);
    EXPECT_EQ(is_equals(ctx.read_buffer, new_correct), true);
    EXPECT_EQ(ctx.number_of_end_reads, 2);
}