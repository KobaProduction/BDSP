#ifndef BDSP_TESTS_UTILS_TESTING_H
#define BDSP_TESTS_UTILS_TESTING_H

#include <cstdint>
#include <vector>

#include <BDSP/streams/types.h>
#include "BDSP/memory_manager.h"


#include "show.h"

namespace {
    class EmptyMemManager final: public BDSP::core::IMemoryManager {
    public:
        void *allocate_memory(size_t size) override { return nullptr; }
        void free_memory(void *ptr) override { }
    };
};

bool is_equals(const uint8_t *data1, size_t size1, const uint8_t *data2, size_t size2);

bool is_equals(const uint8_t *data1, size_t size1, std::vector<uint8_t> data2);

bool is_equals(std::vector<uint8_t> data1, std::vector<uint8_t> data2);

void check_data_for_correctness(std::vector<uint8_t> &current,
                                std::vector<uint8_t> &correct,
                                bool use_hex_when_show = false);

void start_test_writer(BDSP::streams::IStreamWriter &writer,
                       std::vector<uint8_t> &current,
                       std::vector<uint8_t> &correct);

void start_test_reader(BDSP::streams::IStreamReader &reader,
                       std::vector<uint8_t> &current,
                       std::vector<uint8_t> &correct);

void disable_memory_allocation();
void enable_memory_allocation();

#endif // BDSP_TESTS_UTILS_TESTING_H
