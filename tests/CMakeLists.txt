cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
project(BDSP-tests)

#----------------------------------------------------------------------------------------------------------------------
# general settings and options
#----------------------------------------------------------------------------------------------------------------------

include("../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
endif()

#----------------------------------------------------------------------------------------------------------------------
# testing framework
#----------------------------------------------------------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # do not override parent project's runtime settings on Windows
option(INSTALL_GTEST "Build BDSP tests" OFF)

# For simplicity, always build googletest as static library. This prevents BDSP-tests executable from
# complaining about missing googletest DLLs on Windows.
option(BUILD_SHARED_LIBS "Build Shared BDSP" OFF)

FetchContent_MakeAvailable(googletest)

#----------------------------------------------------------------------------------------------------------------------
# tests dependencies
#----------------------------------------------------------------------------------------------------------------------

if(is_top_level)
    find_package(BDSP REQUIRED)
endif()

#----------------------------------------------------------------------------------------------------------------------
# tests sources
#----------------------------------------------------------------------------------------------------------------------

set(sources
        utils/show.h utils/cobs.h
        crc8_test.cpp
        cobs_tests.cpp
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

#----------------------------------------------------------------------------------------------------------------------
# tests target
#----------------------------------------------------------------------------------------------------------------------

add_executable(BDSP-tests)
target_sources(BDSP-tests PRIVATE ${sources})


if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    target_compile_options(BDSP PUBLIC -fprofile-arcs -ftest-coverage)
    target_link_libraries(BDSP PRIVATE gcov)
endif()

target_link_libraries(BDSP-tests PRIVATE BDSP::BDSP gtest_main)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(BDSP-tests BDSP::BDSP)
endif()

include(GoogleTest)
gtest_discover_tests(BDSP-tests)
