#ifndef BDSP_DECODERS_COBS_H
#define BDSP_DECODERS_COBS_H

#include <cstdint>
#include "BDSP/decoders/abs_decoder.h"

#ifndef MIN_BDSP_COBS_DEPTH
#define MIN_BDSP_COBS_DEPTH 16
#endif

namespace BDSP::decoders::COBS {
    using namespace BDSP::decoders;

    struct cobs_config_t {
        uint8_t delimiter;
        uint8_t depth;
    };

    enum fsm_state_t {
        SERVICE_BYTE, REGULAR_BYTE, SWAP_BYTE
    };

    class COBSDecoder final : public ABS::AbstractDecoder {
        cobs_config_t _cfg{};
        fsm_state_t _fsm_state{};
        uint8_t _swap_byte_offset{};
        uint8_t _service_byte_offset{};

        decode_status_t _decode(uint8_t byte) override;

        decode_status_t _set_swap_byte_offset(uint8_t offset);

    public:
        explicit COBSDecoder(cobs_config_t config = {'\0', 255});


        void reset();
    };


}
#endif //BDSP_DECODERS_COBS_H
