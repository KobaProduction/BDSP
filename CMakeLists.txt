# BDSP (Binary Data Separation Protocol)
# https://github.com/KobaProduction/BDSP
# MIT License

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

set(BDSP_SRCS
        src/encoders/cobs/reader.cpp
        src/encoders/cobs/writer.cpp
        src/checksums/crc/crc8.cpp
        src/BDSP.cpp
        src/packet.cpp
        src/utils.cpp
)

idf_component_register(SRCS ${BDSP_SRCS}
        INCLUDE_DIRS "src"
        REQUIRES arduino)

project(BDSP)