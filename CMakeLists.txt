# BDSP (Binary Data Separation Protocol)
# https://github.com/KobaProduction/BDSP
# MIT License

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

project(BDSP
        VERSION 0.1.1
        DESCRIPTION "The library implements a binary data separation protocol."
        LANGUAGES CXX
)

add_subdirectory(src)

if (idf_component_register)
    idf_component_register(
        SRC_DIRS
            src
            src/checksums/crc
            src/encoders/cobs
        INCLUDE_DIRS
            src
            src/checksums/crc
            src/encoders/cobs
        REQUIRES
            arduino
    )
endif()