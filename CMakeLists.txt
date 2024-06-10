# BDSP (Binary Data Separation Protocol)
# https://github.com/ArthurKoba/BDSP
# MIT License

cmake_minimum_required(VERSION 3.5)

set(BDSP_SRCS
        src/Packet.cpp
        src/BDSP.cpp
        src/COBSWriter.cpp
)

idf_component_register(SRCS ${BDSP_SRCS}
        INCLUDE_DIRS "src"
        REQUIRES arduino)

project(BDSP)