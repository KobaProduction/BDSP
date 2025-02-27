#include <gtest/gtest.h>

#include "BDSP/streams/cobs/reader.h"
#include "BDSP/streams/cobs/writer.h"

#include "../../../utils/testing.h"

using namespace BDSP::streams::cobs;
using namespace BDSP::strategies;
using namespace BDSP::strategies::cobs;

namespace {
void is_ready_callback(bool is_ready, void *ctx) {
    *reinterpret_cast<bool *>(ctx) = is_ready;
}
} // namespace

TEST(strategies_cobs_default_tests, set_configuration_test) {
    COBSReadStrategy read_strategy;
    COBSWriteStrategy write_strategy;

    bool read_strategy_ready = false;
    bool write_strategy_ready = false;

    read_strategy.init(nullptr, is_ready_callback, &read_strategy_ready);
    write_strategy.init(nullptr, is_ready_callback, &write_strategy_ready);

    cobs_config_t config = core::COBSConfigsMixin().get_default_config();

    EXPECT_EQ(read_strategy.set_config(config), SET_OK);
    EXPECT_EQ(write_strategy.set_config(config), SET_OK);

    ASSERT_TRUE(read_strategy_ready);
    ASSERT_TRUE(write_strategy_ready);
}

TEST(strategies_cobs_default_tests, set_configuration_with_incorrect_config) {
    COBSReadStrategy read_strategy;
    COBSWriteStrategy write_strategy;

    bool read_strategy_ready = true;
    bool write_strategy_ready = true;

    read_strategy.init(nullptr, is_ready_callback, &read_strategy_ready);
    write_strategy.init(nullptr, is_ready_callback, &write_strategy_ready);

    cobs_config_t config = core::COBSConfigsMixin().get_default_config();

    config.size_of_the_sequence_to_be_replaced = 2;
    EXPECT_EQ(read_strategy.set_config(config), ERROR_DEFAULT_COBS_SIZE_SR);
    EXPECT_EQ(write_strategy.set_config(config), ERROR_DEFAULT_COBS_SIZE_SR);

    ASSERT_FALSE(read_strategy_ready);
    ASSERT_FALSE(write_strategy_ready);
}

TEST(strategies_cobs_default_tests, set_configuration_with_active_process) {
    COBSReadStrategy read_strategy;
    COBSWriteStrategy write_strategy;

    bool read_strategy_ready = true;
    bool write_strategy_ready = true;

    read_strategy.init(nullptr, is_ready_callback, &read_strategy_ready);
    write_strategy.init(nullptr, is_ready_callback, &write_strategy_ready);

    cobs_config_t config = core::COBSConfigsMixin().get_default_config();

    write_strategy.write(0xAA);
    read_strategy.read(0x01);

    EXPECT_EQ(read_strategy.set_config(config), ERROR_PROCESS_NOT_FINISHED);
    EXPECT_EQ(write_strategy.set_config(config), ERROR_PROCESS_NOT_FINISHED);

    ASSERT_FALSE(read_strategy_ready);
    ASSERT_FALSE(write_strategy_ready);

    read_strategy.reset_read_state();
    class AccessFieldClass: public core::COBSReadStrategyCore {
    public:
        using core::COBSReadStrategyCore::_offset_to_service_byte;
    };
    reinterpret_cast<AccessFieldClass *>(&read_strategy)->_offset_to_service_byte = 1;

    EXPECT_EQ(read_strategy.set_config(config), ERROR_PROCESS_NOT_FINISHED);
    ASSERT_FALSE(read_strategy_ready);
}

TEST(strategies_cobs_default_tests, set_configuration_to_write_strategy_when_no_free_memory) {
    COBSWriteStrategy write_strategy;

    bool write_strategy_ready = true;

    write_strategy.init(nullptr, is_ready_callback, &write_strategy_ready);

    class AccessFieldClass: public core::COBSWriteStrategyCore {
    public:
        using core::COBSWriteStrategyCore::_malloc;
    };
    reinterpret_cast<AccessFieldClass *>(&write_strategy)->_malloc = [](size_t size) -> void * { return nullptr; };

    cobs_config_t config = write_strategy.get_config();
    config.depth--;

    EXPECT_EQ(write_strategy.set_config(config), ERROR_MEMORY_ALLOCATION);

    ASSERT_FALSE(write_strategy_ready);
}

TEST(strategies_cobs_default_tests, reader_errors_test) {
    COBSReadStrategy read_strategy;
    cobs_config_t config = read_strategy.get_config();

    std::vector<strategy_read_status_t> statuses;

    read_strategy.init(
        [](uint8_t byte, strategy_read_status_t status, void *ctx) {
            reinterpret_cast<std::vector<strategy_read_status_t> *>(ctx)->push_back(status);
        },
        nullptr,
        &statuses);

    EXPECT_EQ(read_strategy.read(config.delimiter_byte + 2), STRATEGY_READ_OK);
    ASSERT_TRUE(statuses.empty());

    EXPECT_EQ(read_strategy.read(config.delimiter_byte), STRATEGY_READ_DELIMITER);
    ASSERT_TRUE(statuses == std::vector<strategy_read_status_t>({STRATEGY_READ_ERROR, STRATEGY_READ_DELIMITER}));

    config.delimiter_byte = 1;
    config.depth = 16;

    EXPECT_EQ(read_strategy.set_config(config), SET_OK);
    statuses.clear();
    EXPECT_EQ(read_strategy.read(0x00), STRATEGY_READ_OK);
    EXPECT_EQ(read_strategy.read(config.depth + 1), STRATEGY_READ_ERROR);

    ASSERT_TRUE(statuses == std::vector<strategy_read_status_t>({STRATEGY_READ_ERROR}));
}

TEST(strategies_cobs_default_tests, encoding_depth_test) {
    COBSReaderStream reader;
    COBSWriterStream writer;

    cobs_config_t config = core::COBSConfigsMixin().get_default_config();
    config.depth = 16;
    EXPECT_EQ(reader.get_strategy().set_config(config), SET_OK);
    EXPECT_EQ(writer.get_strategy().set_config(config), SET_OK);

    std::vector<uint8_t> data;
    std::vector<uint8_t> correct_encoded = {config.depth,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            255,
                                            1,
                                            config.delimiter_byte};

    data.reserve(config.depth - 1);
    for (int i = 0; i < config.depth - 1; ++i) {
        data.push_back(255);
    }

    start_test_writer(writer, data, correct_encoded);
    start_test_reader(reader, correct_encoded, data);

    data.push_back(255);

    correct_encoded = {config.depth,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       255,
                       2,
                       255,
                       config.delimiter_byte};

    start_test_writer(writer, data, correct_encoded);
    start_test_reader(reader, correct_encoded, data);
}

TEST(strategies_cobs_default_tests, encoding_custom_delimiter_test) {
    COBSReaderStream reader;
    COBSWriterStream writer;

    cobs_config_t config = core::COBSConfigsMixin().get_default_config();
    config.delimiter_byte = 2;
    EXPECT_EQ(reader.get_strategy().set_config(config), SET_OK);
    EXPECT_EQ(writer.get_strategy().set_config(config), SET_OK);

    std::vector<uint8_t> data = {0, 2, 0, 0};
    std::vector<uint8_t> correct_encoded = {0, 0, 3, 0, 0, 2};

    start_test_reader(reader, correct_encoded, data);
    start_test_writer(writer, data, correct_encoded);
}