#include <Arduino.h>

#if not defined(SERIAL_SPEED)
#define SERIAL_SPEED 115200
#endif

#include "BDSP/strategies/cobs/write.h"
#include "BDSP/strategies/cobs/read.h"
#include "BDSP/strategies/byte_stuffing/write.h"
#include "BDSP/strategies/byte_stuffing/read.h"

using namespace BDSP::strategies;

cobs::COBSZPEWriteStrategy write_strategy;
cobs::COBSZPEReadStrategy read_strategy;

//byte_stuffing::PPPWriteStrategy write_strategy;
//byte_stuffing::PPPReadStrategy read_strategy;

enum Strategy {
    ZERO, BYTE_7E, CYCLE, CYCLE_PAIRING
};
const uint32_t count_iteration = 3;

uint32_t max_data_size = 65536 * 8;

uint8_t encoded_buffer[1100];

struct TestResult {
    float bytes_per_us = 0.0;
    uint32_t wrote_bytes = 0;
    uint64_t test_delta_time_us = 0;

    TestResult() = default;

    TestResult(float bytes_per_us_, uint32_t wrote_bytes_, uint64_t test_delta_time_us_) {
        bytes_per_us = bytes_per_us_;
        wrote_bytes = wrote_bytes_;
        test_delta_time_us = wrote_bytes_;
        test_delta_time_us = test_delta_time_us_;
    }
};


void write_bytes(IStreamWriteStrategy *w_strategy, uint32_t data_size, Strategy strategy) {
    if (strategy == ZERO) {
        for (uint32_t i = 0; i < data_size; ++i) {
            w_strategy->write(0);
        }
    } else if (strategy == BYTE_7E) {
        for (uint32_t i = 0; i < data_size; ++i) {
            w_strategy->write(0x7E);
        }
    } else if (strategy == CYCLE) {
        for (uint32_t i = 0; i < data_size; ++i) {
            w_strategy->write(i);
        }
    } else if (strategy == CYCLE_PAIRING) {
        for (uint32_t i = 0; i < data_size / 2; ++i) {
            w_strategy->write(i);
            w_strategy->write(i);
        }
    }
    w_strategy->finish();
}

TestResult run_write_test(IStreamWriteStrategy *w_strategy, uint32_t data_size, Strategy strategy) {
    uint32_t counter = 0;
    w_strategy->init([](uint8_t byte, void *ctx) {
//        Serial.write(byte);
//        Serial.flush();
        auto &counter = *reinterpret_cast<size_t *>(ctx);
        counter++;
    }, nullptr, &counter);
    auto timer_us = micros();
    write_bytes(w_strategy, data_size, strategy);
    timer_us = micros() - timer_us;
    float bytes_per_us = float(data_size) / float(timer_us);
    return {bytes_per_us, counter, timer_us};
}


void run_write_tests() {
    for (int i = 0; i <= static_cast<int>(CYCLE_PAIRING); ++i) {
//        Serial.print(F("Run write test use size: "));
//        Serial.print(max_data_size);
//        Serial.print(F(", and type: "));
//        Serial.print(i);
//        Serial.print(F(" - "));
//        Serial.flush();
        float avg_timer = 0;
        float avg_bytes_per_us = 0;
        run_write_test(&write_strategy, UINT8_MAX, CYCLE);
        for (int j = 0; j < count_iteration; ++j) {
            delayMicroseconds(100);
            auto result = run_write_test(&write_strategy, max_data_size, static_cast<Strategy>(i));
//                printf("End write %u! Wrote bytes: %u, delta time (us): %lu, bytes per us: %.1f \n",
//                              data_size, counter, timer_us, bytes_per_us
//                );
            avg_timer += float(result.test_delta_time_us);
            avg_bytes_per_us += result.bytes_per_us;
        }
        avg_timer /= count_iteration;
        avg_bytes_per_us /= count_iteration;
        Serial.print(F("avg time: "));
        Serial.print(avg_timer, 3);
        Serial.print(F(", avg bytes_per_us: "));
        Serial.print(avg_bytes_per_us, 3);
        Serial.println();
    }
}


TestResult run_reader_test(IStreamReadStrategy *r_strategy, uint8_t *encoded_data, uint32_t encoded_data_size) {
    uint32_t counter = 0;
    r_strategy->init([](uint8_t byte, strategy_read_status_t status, void *ctx) {
        auto &counter = *reinterpret_cast<uint32_t *>(ctx);
        counter++;
    }, nullptr, &counter);
    auto timer_us = micros();
    for (uint32_t i = 0; i < encoded_data_size; ++i) {
        r_strategy->read(encoded_data[i]);
    }
    timer_us = micros() - timer_us;
    return {float(encoded_data_size) / float(timer_us), counter, timer_us};
}

uint32_t prepare_buffer(IStreamWriteStrategy *w_strategy, uint32_t data_size, Strategy strategy) {
    uint32_t counter = 0;
    w_strategy->init([](uint8_t byte, void *ctx) {
        auto &counter = *reinterpret_cast<uint32_t *>(ctx);
        encoded_buffer[counter] = byte;
        counter++;
    }, nullptr, &counter);
    write_bytes(w_strategy, data_size, strategy);
    return counter;
}


void show_buffer(uint8_t *buffer, uint32_t size) {
    if (not size) {
        Serial.println(F("Buffer empty."));
        return;
    }
    Serial.print(int(buffer[0]));
    for (uint32_t i = 1; i < size; ++i) {
        Serial.print(',');
        Serial.print(int(buffer[i]));
        Serial.flush();
    }
    Serial.print('\n');
}

void run_read_tests() {
    for (int i = 0; i <= static_cast<int>(CYCLE_PAIRING); ++i) {
        size_t encoded_size = prepare_buffer(&write_strategy, 512, static_cast<Strategy>(i));
//        Serial.print(F("Run read test use size: "));
//        Serial.print(max_data_size);
//        Serial.print(F(", and type: "));
//        Serial.print(i);
//        Serial.print(F(" - buffer prepared (size: "));
//        Serial.print(encoded_size);
//        Serial.print(F(") - "));
//        Serial.flush();

        uint32_t success_read = 0;
        float avg_timer = 0;
        float avg_bytes_per_us = 0;
        auto result = run_reader_test(&read_strategy, encoded_buffer, encoded_size);
        uint32_t count_iterations = 0;
        while(success_read < max_data_size) {
            result = run_reader_test(&read_strategy, encoded_buffer, encoded_size);
            success_read += result.wrote_bytes;
            avg_timer += float(result.test_delta_time_us);
            avg_bytes_per_us += result.bytes_per_us;
            count_iterations++;
        }
        avg_timer /= float(count_iterations);
        avg_bytes_per_us /= float(count_iterations);
        Serial.print(F("success read: "));
        Serial.print(success_read);
        Serial.print(F(", avg bytes_per_us: "));
        Serial.print(avg_bytes_per_us, 3);
        Serial.println();
        Serial.flush();
    }
}

void setup() {
    Serial.begin(SERIAL_SPEED);
    Serial.println(F("\nStart base-embedded-project."));

    Serial.println(F("start bdsp strategies tests"));
    run_write_tests();
    run_read_tests();
    Serial.println(F("tests finished"));

}

void loop() {
    while (true);
}