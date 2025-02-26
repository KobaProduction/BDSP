#include <gtest/gtest.h>

#include "BDSP/streams/cobs/reader.h"
#include "BDSP/streams/cobs/writer.h"

#include "../../../../utils/testing.h"

using namespace BDSP::streams::cobs;
using namespace BDSP::streams::strategies;
using namespace BDSP::streams::strategies::cobs;

TEST(strategies_strategies_cobs_sr_tests, set_configuration_with_active_process) {

    class AccessFieldClass: public core::COBSSRWriteStrategyCore {
    public:
        using core::COBSSRWriteStrategyCore::_counter_of_the_sequence_to_be_replaced;
    };

    COBSSRWriteStrategy write_strategy;
    COBSSRReadStrategy read_strategy;

    cobs_config_t config = core::COBSSRConfigsMixin().get_default_config();

    reinterpret_cast<AccessFieldClass*>(&write_strategy)->_counter_of_the_sequence_to_be_replaced = 1;

    EXPECT_EQ(write_strategy.set_config(config), ERROR_PROCESS_NOT_FINISHED);
    reinterpret_cast<AccessFieldClass*>(&write_strategy)->_counter_of_the_sequence_to_be_replaced = 0;

    read_strategy.read(config.depth + 1);
    EXPECT_EQ(read_strategy.set_config(config), ERROR_PROCESS_NOT_FINISHED);
}

TEST(strategies_cobs_sr_tests, reader_errors_test) {
    cobs_config_t config = core::COBSSRConfigsMixin().get_default_config();

    COBSSRReadStrategy read_strategy;
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

    statuses.clear();
    EXPECT_EQ(read_strategy.read(128), STRATEGY_READ_OK);
    ASSERT_TRUE(statuses.empty());
    EXPECT_EQ(read_strategy.read(config.delimiter_byte), STRATEGY_READ_DELIMITER);
    ASSERT_TRUE(statuses == std::vector<strategy_read_status_t>({STRATEGY_READ_ERROR, STRATEGY_READ_DELIMITER}));

    statuses.clear();
    EXPECT_EQ(read_strategy.read(128), STRATEGY_READ_OK);
    EXPECT_EQ(read_strategy.read(config.depth * 2 + 1), STRATEGY_READ_ERROR);
    ASSERT_TRUE(statuses == std::vector<strategy_read_status_t>({STRATEGY_READ_ERROR}));
}

TEST(strategies_cobs_sr_tests, writer_reset_elimination_sequence_test) {
    COBSSRReaderStream reader;
    COBSSRWriterStream writer;

    cobs_config_t config = core::COBSSRConfigsMixin().get_default_config();
    config.delimiter_byte = 255;
    writer.get_strategy().set_config(config);
    reader.get_strategy().set_config(config);

    std::vector<uint8_t> data = {config.byte_of_the_sequence_to_be_replaced};
    std::vector<uint8_t> correct_encoded = {2, config.byte_of_the_sequence_to_be_replaced, config.delimiter_byte};
    start_test_writer(writer, data, correct_encoded);
    start_test_reader(reader, correct_encoded, data);

    data = {config.byte_of_the_sequence_to_be_replaced, 1};
    correct_encoded = {3, config.byte_of_the_sequence_to_be_replaced, 1, config.delimiter_byte};
    start_test_writer(writer, data, correct_encoded);
    start_test_reader(reader, correct_encoded, data);

    data = {1, 1};
    correct_encoded = {3, 1, 1, config.delimiter_byte};
    start_test_writer(writer, data, correct_encoded);
    start_test_reader(reader, correct_encoded, data);
}

TEST(strategies_cobs_sr_tests, sequence_replacement_test) {
    COBSSRReaderStream reader;
    COBSSRWriterStream writer;

    std::vector<uint8_t> data = {0x01, 0x00, 0x00, 0x00, 0x01};
    std::vector<uint8_t> correct_encoded = {129, 0x01, 0x01, 0x02, 0x01, 0x00};
    start_test_writer(writer, data, correct_encoded);
    start_test_reader(reader, correct_encoded, data);
}

TEST(strategies_cobs_sr_tests, reset_sequence_replacement_state) {
    COBSSRReadStrategy cobs_reader;

    std::vector<uint8_t> encoded = {128, 0x00, 0x00, 128, 0x01, 0x00};
    std::vector<uint8_t> correct_decoded = {0x00, 0x00};
    std::vector<uint8_t> decoded = {};

    strategy_data_callback_t cb = [](uint8_t byte, strategy_read_status_t status, void *ctx) {
        if (status not_eq STRATEGY_READ_OK) {
            return;
        }
        reinterpret_cast<std::vector<uint8_t> *>(ctx)->push_back(byte);
    };
    cobs_reader.init(cb, nullptr, &decoded);

    for (unsigned char i : encoded) {
        cobs_reader.read(i);
    }

    check_data_for_correctness(decoded, correct_decoded);
}