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
    };

    class COBSEncoder final : public ABS::AbstractEncoder {
        COBS::cobs_config_t _cfg{};
        uint8_t *_buffer_ptr = nullptr;
        uint8_t _buffer_position = 1;

        void _encode(uint8_t byte) override;

        void _finish_encode() override;

        void _write_buffer();

    public:
        explicit COBSEncoder(COBS::cobs_config_t config = {'\0', 255});

        ~COBSEncoder();
    };
}

#endif //BDSP_DECODERS_PPP_H
