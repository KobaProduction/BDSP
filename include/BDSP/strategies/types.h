#ifndef BDSP_STREAMS_STRATEGIES_TYPES_H
#define BDSP_STREAMS_STRATEGIES_TYPES_H

#include <stdint.h>

namespace BDSP::strategies {

enum strategy_read_status_t { STRATEGY_READ_OK = 0, STRATEGY_READ_DELIMITER = 1, STRATEGY_READ_ERROR = 2 };

typedef void (*strategy_data_callback_t)(uint8_t byte, strategy_read_status_t status, void *context_ptr);
typedef void (*strategy_ready_state_callback_t)(bool state, void *ctx);
typedef void (*strategy_write_handler_t)(uint8_t byte, void *context_ptr);

class IStreamReadStrategy {
public:
    virtual void init(strategy_data_callback_t read_callback,
                      strategy_ready_state_callback_t ready_callback,
                      void *ctx) = 0;
    virtual strategy_read_status_t read(uint8_t byte) = 0;
    virtual void reset_read_state() = 0;
    virtual ~IStreamReadStrategy() = default;
};

class IStreamWriteStrategy {
public:
    virtual void init(strategy_write_handler_t write_callback,
                      strategy_ready_state_callback_t ready_callback,
                      void *ctx) = 0;
    virtual void finish() = 0;
    virtual void send_delimiter() = 0;
    virtual void write(uint8_t byte) = 0;
    virtual ~IStreamWriteStrategy() = default;
};

} // namespace BDSP::strategies

#endif // BDSP_STREAMS_STRATEGIES_TYPES_H
