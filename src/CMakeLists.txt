set(BDSP_SRCS
        BDSP.h
        bdsp_types.h
        packet.h packet.cpp
        bdsp_mixins.cpp
        bdsp_receiver.h bdsp_receiver.cpp
        bdsp_transceiver.h bdsp_transceiver.cpp
        bdsp_transmitter.h bdsp_transmitter.cpp
        encoders/cobs/decoder.h encoders/cobs/decoder.cpp
        encoders/cobs/encoder.h encoders/cobs/encoder.cpp
        checksums/crc/crc8.h checksums/crc/crc8.cpp
)

add_library(BDSP ${BDSP_SRCS})