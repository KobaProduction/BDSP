#ifndef BDSP_DECODERS_ABS_H
#define BDSP_DECODERS_ABS_H

#include <cstdint>

#include <BDSP/decoders/types.h>
#include <BDSP/decoders/interface.h>

namespace BDSP::decoders::ABS {
    using namespace BDSP::decoders;

    class AbstractDecoder : public IDecoder {
    private:
        data_handler_t _data_handler = nullptr;
        void *_data_handler_context = nullptr;
        bool _is_waiting_for_the_delemiter = false;
    protected:

        virtual decode_status_t _decode(uint8_t byte) = 0;

        void _handler(uint8_t byte, decode_status_t state);

        virtual void _reset() = 0;

    public:

        decode_status_t decode(uint8_t byte) final;

        decode_status_t decode(uint8_t *buffer_ptr, size_t size) final;

        void reset_decode_state(bool is_need_wait_delemiter) final;

        void set_data_handler(data_handler_t handler, void *context_ptr) final;
    };
}

#endif //BDSP_DECODERS_ABS_H
