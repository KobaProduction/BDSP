#include <gtest/gtest.h>

#include "BDSP/streams/COBS/checkers.h"
#include "BDSP/streams/COBS/reader.h"
#include "BDSP/streams/COBS/writer.h"

#include "../../utils/cobs.h"
#include "../../utils/show.h"
#include "../../utils/testing.h"

using namespace BDSP::streams;
using namespace BDSP::streams::COBS;
using namespace BDSP::streams::COBS::core;

TEST(cobs_tests, cobs_reference_encoders_test) {
    std::vector<uint8_t> data;
    std::vector<uint8_t> encoded;
    std::vector<uint8_t> decoded;

    for (int i = 0; i < 512; ++i) {
        data.push_back(i);
    }
    cobs_encode(data, encoded);
    cobs_decode(encoded, decoded);
    if (is_equals(data, decoded)) {
        return;
    }
    std::cout << "Correct ";
    show_data(data);
    std::cout << "Encoded ";
    show_data(encoded);
    std::cout << "Decoded ";
    show_data(decoded);
    FAIL() << "the correct and encoded array is not equal";
}

TEST(cobs_tests, cobs_default_config_checker_test) {

    cobs_config_t config = {.delimiter = '\0',
                            .depth = 255,
                            .size_of_the_sequence_to_be_replaced = 0,
                            .byte_of_the_sequence_to_be_replaced = '\0'};

    EXPECT_EQ(cobs_default_config_checker(config), SET_OK);

    config.size_of_the_sequence_to_be_replaced = 2;

    EXPECT_EQ(cobs_default_config_checker(config), ERROR_DEFAULT_COBS_SIZE_SR);
}

TEST(cobs_tests, cobs_sr_config_checker_test) {

    cobs_config_t config = {.delimiter = '\0',
                            .depth = 127,
                            .size_of_the_sequence_to_be_replaced = 2,
                            .byte_of_the_sequence_to_be_replaced = '\0'};

    EXPECT_EQ(cobs_sr_config_checker(config), SET_OK);

    config.depth = 255;

    EXPECT_EQ(cobs_sr_config_checker(config), ERROR_DEPTH_SR);

    config.size_of_the_sequence_to_be_replaced = 0;

    EXPECT_EQ(cobs_sr_config_checker(config), ERROR_SIZE_SR);
}

TEST(cobs_tests, cobs_zpe_config_checker_test) {

    cobs_config_t config = {.delimiter = '\0',
                            .depth = 224,
                            .size_of_the_sequence_to_be_replaced = 2,
                            .byte_of_the_sequence_to_be_replaced = '\0'};

    EXPECT_EQ(cobs_zpe_config_checker(config), SET_OK);

    config.depth = 255;

    EXPECT_EQ(cobs_zpe_config_checker(config), ERROR_DEPTH_ZPE);

    config.size_of_the_sequence_to_be_replaced = 0;

    EXPECT_EQ(cobs_zpe_config_checker(config), ERROR_SIZE_SR);
}

TEST(cobs_tests, cobs_writer_set_configuration_test) {
    class COBSWriterTest: public COBSWriterCore {
    public:
        void set_allocator(void *(*allocator)(size_t)) { _malloc = allocator; }
        void set_buffer_position(uint8_t value) {
            _buffer_position = value;
        }
    };

    COBSWriterTest cobs_writer;
    cobs_config_t config = cobs_writer.get_config();

    cobs_writer.set_allocator([](size_t size) -> void * { return nullptr; });
    EXPECT_EQ(cobs_writer.set_config(config), ERROR_MEMORY_ALLOCATION);

    cobs_writer.set_allocator(malloc);

    EXPECT_EQ(cobs_writer.set_config(config), SET_OK);
    EXPECT_EQ(cobs_writer.set_config(config), SET_OK);

    cobs_writer.set_buffer_position(2);
    EXPECT_EQ(cobs_writer.set_config(config), ERROR_PROCESS_NOT_FINISHED);
    cobs_writer.set_buffer_position(1);

    config.depth = 1;
    EXPECT_EQ(cobs_writer.set_config(config), ERROR_COBS_DEPTH);
}

TEST(cobs_tests, cobs_reader_set_configuration_test) {
    class COBSReaderTest : public COBSReaderCore {
    public:

        void set_fst_state(fsm_state_t state) {
            _fsm_state = state;
        }

        void set_service_byte_offset(uint8_t offset) {
            _service_byte_offset = offset;
        }
    };

    COBSReaderTest cobs_reader;
    cobs_config_t config = cobs_reader.get_config();

    EXPECT_EQ(cobs_reader.set_config(config), SET_OK);

    cobs_reader.set_fst_state(REGULAR_BYTE);
    EXPECT_EQ(cobs_reader.set_config(config), ERROR_PROCESS_NOT_FINISHED);
    cobs_reader.set_fst_state(SERVICE_BYTE);

    cobs_reader.set_service_byte_offset(config.depth + 1);
    EXPECT_EQ(cobs_reader.set_config(config), ERROR_PROCESS_NOT_FINISHED);
    cobs_reader.set_service_byte_offset(config.depth);

    config.depth = 1;
    EXPECT_EQ(cobs_reader.set_config(config), ERROR_COBS_DEPTH);
}




