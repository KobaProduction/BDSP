#include <cstdint>
#include <gtest/gtest.h>

#include <BDSP/encoders/abs_encoder.h>
#include <BDSP/decoders/abs_decoder.h>

using namespace BDSP::encoders::ABS;
using namespace BDSP::decoders::ABS;

TEST(abstract_encoding_tests, abstract_encoder_full_test) {
    class SimpleEncoder : public AbstractEncoder {
        void _encode(uint8_t byte) override { _write(&byte, 1); };

        encode_status_t finish_encode() override { return ENCODE_FINISH; };
    public:
        void toggle_ready() { _is_ready = !_is_ready; }
    };

    auto encoder = SimpleEncoder();

    uint8_t data[2];
    auto status = encoder.encode(data, 2);
    EXPECT_EQ(status, UNKNOWN_ENCODER_ERROR);
    status = encoder.encode(data[0]);
    EXPECT_EQ(status, UNKNOWN_ENCODER_ERROR);

    encoder.set_writer([](uint8_t byte, void *ctx) {});
    status = encoder.encode(data, 2);
    EXPECT_EQ(status, DECODE_OK);


    encoder.toggle_ready();
    status = encoder.encode(data[0]);
    EXPECT_EQ(status, UNKNOWN_ENCODER_ERROR);
}

TEST(abstract_encoding_tests, abstract_decoder_full_test) {
    class SimpleDecoder : public AbstractDecoder {
        decode_status_t _decode(uint8_t byte) override {
            decode_status_t status = DECODE_OK;
            switch (byte) {
                case 0x00:
                    status = DECODE_END;
                    break;
                case 0x01:
                    status = DECODE_ERROR;
                    break;
                default:
                    break;
            }
            _handler(byte, status);
            return status;
        }
    };

    auto decoder = SimpleDecoder();

    uint8_t data[2] = {0x00, 0x00};
    auto status = decoder.decode(data, 2);
    EXPECT_EQ(status, UNKNOWN_DECODER_ERROR);

    decoder.set_data_handler([](uint8_t byte, decode_status_t status, void *ctx) {});

    status = decoder.decode(data, 2);
    EXPECT_EQ(status, DECODE_OK);
    data[0] = 0x01;

    status = decoder.decode(data, 2);
    EXPECT_EQ(status, DECODE_ERROR);

}