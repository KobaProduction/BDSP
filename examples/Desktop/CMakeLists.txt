cmake_minimum_required(VERSION 3.14)
project(BDSP-Desktop LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include("../../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(BDSP REQUIRED)
endif()

set(sources
        ${CMAKE_SOURCE_DIR}/tests/utils/show.h)
#source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(BDSP-Desktop-PPP _PPP.cpp)
target_sources(BDSP-Desktop-PPP PRIVATE ${sources})
target_link_libraries(BDSP-Desktop-PPP PRIVATE BDSP::BDSP)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(BDSP-Desktop-PPP BDSP::BDSP)
endif()
