#ifndef BDSP_STREAMS_TYPES_H
#define BDSP_STREAMS_TYPES_H

#include <cstdint>

namespace BDSP::streams {

    typedef void (*write_handler_t)(uint8_t byte, void *write_handler_context_ptr);

    enum write_status_t {
        WRITE_OK, WRITE_FINISH, UNKNOWN_WRITER_ERROR
    };

    class IWriter {
    public:
        virtual write_status_t write(uint8_t byte) = 0;

        virtual write_status_t write(uint8_t *buffer_ptr, size_t size) = 0;

        virtual write_status_t finish() = 0;

        virtual void set_writer(write_handler_t writer, void *context_ptr) = 0;

    };


    enum read_status_t {
        READ_OK, READ_END, READ_ERROR, UNKNOWN_READER_ERROR
    };

    typedef void (*data_handler_t)(uint8_t byte, read_status_t status, void *data_handler_context_ptr);

    class IReader {
    public:
        virtual void set_data_handler(data_handler_t handler, void *context_ptr) = 0;

        virtual void reset_read_state(bool need_wait_delemiter) = 0;

        virtual read_status_t read(uint8_t byte) = 0;

        virtual read_status_t read(uint8_t *buffer_ptr, size_t size) = 0;

    };

}

#endif //BDSP_STREAMS_TYPES_H
