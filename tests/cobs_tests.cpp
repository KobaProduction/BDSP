#include <gtest/gtest.h>

#include <BDSP/streams/COBS/writer.h>
#include <BDSP/streams/COBS/reader.h>

#include "utils/cobs.h"
#include "utils/testing.h"
#include "utils/show.h"

using namespace BDSP::streams;

TEST(cobs_pipelines_tests, cobs_utils_test) {
    std::vector<uint8_t> data;
    std::vector<uint8_t> encoded;
    std::vector<uint8_t> decoded;

    for (int i = 0; i < 512; ++i) data.push_back(i);
    cobs_encode(data, encoded);
    cobs_decode(encoded, decoded);
    if (is_equals(data, decoded)) return;

    std::cout << "Correct ";
    show_data(data);
    std::cout << "Encoded ";
    show_data(encoded);
    std::cout << "Decoded ";
    show_data(decoded);
    FAIL() << "the correct and encoded array is not equal";
}

TEST(cobs_pipelines_tests, encoding_default_test) {
    COBS::COBSWriter cobs_writer;

    std::vector<uint8_t> data;
    std::vector<uint8_t> correct_encoded;

    for (int size = 1; size < 1000; ++size) {
        data.clear();
        correct_encoded.clear();

        for (int i = 0; i < size; ++i) data.push_back(i);
        cobs_encode(data, correct_encoded);
        start_test_writer(cobs_writer, data, correct_encoded);
    }
}

TEST(cobs_pipelines_tests, decoding_test) {
    COBS::COBSReader cobs_reader;

    std::vector<uint8_t> data;
    std::vector<uint8_t> encoded;

    for (int size = 1; size < 1000; ++size) {
        data.clear();
        encoded.clear();
        for (int i = 0; i < size; ++i) data.push_back(i);
        cobs_encode(data, encoded);
        start_test_reader(cobs_reader, encoded, data);
    }
}

TEST(cobs_pipelines_tests, encoding_custom_delimiter_test) {
    COBS::COBSWriter cobs_writer;
    cobs_writer.set_config({.delimiter = '\n'});
    COBS::COBSReader cobs_reader({.delimiter = '\n'});

    struct Context {
        std::vector<uint8_t> data;
        std::vector<uint8_t> encoded;
        std::vector<uint8_t> decoded;
        bool is_ended = false;
    } ctx;

    cobs_writer.set_stream_writer([](uint8_t byte, void *ctx) {
        reinterpret_cast<Context *>(ctx)->encoded.push_back(byte);
    }, &ctx);

    cobs_reader.set_stream_data_handler([](uint8_t byte, read_status_t status, void *ctx) {
        auto &context = *reinterpret_cast<Context *>(ctx);
        ASSERT_FALSE(context.is_ended);
        if (status == READ_OK) context.decoded.push_back(byte);
        if (status == READ_END) context.is_ended = true;
    }, &ctx);

    for (int size = 1; size < 1000; ++size) {
        ctx.data.clear();
        ctx.encoded.clear();
        ctx.decoded.clear();

        for (int i = 0; i < size; ++i) ctx.data.push_back(i);
        cobs_writer.write(ctx.data.data(), ctx.data.size());
        cobs_writer.finish();
        cobs_reader.read(ctx.encoded.data(), ctx.encoded.size());
        ASSERT_TRUE(is_equals(ctx.data, ctx.decoded));
        ASSERT_TRUE(ctx.is_ended);
        ctx.is_ended = false;
    }
}

TEST(cobs_pipelines_tests, cobs_with_sequence_replacement_test) {
    COBS::COBSWriter cobs_writer;
    cobs_writer.set_config({.delimiter = 0, .size_of_the_sequence_to_be_replaced = 2});
    COBS::COBSReader cobs_reader({.delimiter = 0, .size_of_the_sequence_to_be_replaced = 2});

    struct Context {
        std::vector<uint8_t> data;
        std::vector<uint8_t> encoded;
        std::vector<uint8_t> decoded;
        bool is_ended = false;
    } ctx;

    cobs_writer.set_stream_writer([](uint8_t byte, void *ctx) {
        reinterpret_cast<Context *>(ctx)->encoded.push_back(byte);
    }, &ctx);

    cobs_reader.set_stream_data_handler([](uint8_t byte, read_status_t status, void *ctx) {
        auto &context = *reinterpret_cast<Context *>(ctx);
        ASSERT_FALSE(context.is_ended);
        if (status == READ_OK) context.decoded.push_back(byte);
        if (status == READ_END) context.is_ended = true;
    }, &ctx);

    ctx.data = {0x00, 0x00, 0x00, 0x01};
    cobs_writer.write(ctx.data.data(), ctx.data.size());
    cobs_writer.finish();
    std::vector<uint8_t> correct_encoded = {127 + 1, 0x00, 0x00, 0x01};
    cobs_reader.read(ctx.encoded.data(), ctx.encoded.size());
    ASSERT_TRUE(is_equals(ctx.data, ctx.decoded));
    ASSERT_TRUE(ctx.is_ended);
    ctx.is_ended = false;
}