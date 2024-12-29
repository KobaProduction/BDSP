#ifndef BDSP_DECODERS_PPP_H
#define BDSP_DECODERS_PPP_H

#include <BDSP/encoders/abs_encoder.h>

#ifndef MIN_BDSP_COBS_DEPTH
#define MIN_BDSP_COBS_DEPTH 16
#endif

namespace BDSP::encoders::COBS {
    using namespace BDSP::encoders;

    struct cobs_config_t {
        uint8_t delimiter;
        uint8_t depth;
        uint8_t size_of_the_sequence_to_be_replaced;
        uint8_t byte_of_the_sequence_to_be_replaced;
    };


    class COBSEncoder final : public ABS::AbstractEncoder {
        COBS::cobs_config_t _cfg{};
        uint8_t *_buffer_ptr = nullptr;
        uint8_t _buffer_position = 1;
        uint8_t _current_size_of_the_sequence_to_be_replaced = 0;

        void _encode_default(uint8_t byte);

        void _encode(uint8_t byte) override;

        void _finish_encode() override;

        void _reset_elimination_sequence();

        void _write_buffer(bool is_elimination_sequence = false);

    public:
        explicit COBSEncoder(COBS::cobs_config_t config = {'\0', 255});

        ~COBSEncoder();
    };
}

#endif //BDSP_DECODERS_PPP_H
