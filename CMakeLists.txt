cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
project(BDSP
        VERSION 0.2.1
        DESCRIPTION "The library implements a binary data separation protocol."
        LANGUAGES CXX
)

#----------------------------------------------------------------------------------------------------------------------
# general settings and options
#----------------------------------------------------------------------------------------------------------------------

include(cmake/utils.cmake)
include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

# BDSP_SHARED_LIBS option (undefined by default) can be used to force shared/static build
option(BDSP_BUILD_TESTS "Build BDSP tests" OFF)
option(BDSP_BUILD_EXAMPLES "Build BDSP examples" OFF)
option(BDSP_BUILD_DOCS "Build BDSP documentation" OFF)
option(BDSP_INSTALL "Generate target for installing BDSP" ${is_top_level})
set_if_undefined(BDSP_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/BDSP" CACHE STRING
        "Install path for BDSP package-related CMake files")

if (DEFINED BDSP_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${BDSP_SHARED_LIBS})
endif ()

if (NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo" "Coverage")
endif ()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(BDSP) # initialized below
add_library(BDSP::BDSP ALIAS BDSP)

#----------------------------------------------------------------------------------------------------------------------
# BDSP dependencies
#----------------------------------------------------------------------------------------------------------------------

# Search for your dependencies here

#----------------------------------------------------------------------------------------------------------------------
# BDSP sources
#----------------------------------------------------------------------------------------------------------------------

include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if (NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif ()

generate_export_header(BDSP EXPORT_FILE_NAME include/BDSP/${export_file_name})

set(headers
        include/BDSP/BDSP.h
        include/BDSP/export.h
        include/BDSP/mixins.h
        include/BDSP/receiver.h
        include/BDSP/transceiver.h
        include/BDSP/transmitter.h
        include/BDSP/types.h

        include/BDSP/checksums/crc8.h

        include/BDSP/streams/mixins.h
        include/BDSP/streams/reader.h
        include/BDSP/streams/writer.h
        include/BDSP/streams/types.h

        include/BDSP/streams/cobs/reader.h
        include/BDSP/streams/cobs/writer.h

        include/BDSP/streams/ppp/reader.h
        include/BDSP/streams/ppp/writer.h

        include/BDSP/streams/strategies/types.h

        include/BDSP/streams/strategies/abstract/read.h
        include/BDSP/streams/strategies/abstract/write.h

        include/BDSP/streams/strategies/byte_stuffing/mixins.h
        include/BDSP/streams/strategies/byte_stuffing/read.h
        include/BDSP/streams/strategies/byte_stuffing/types.h
        include/BDSP/streams/strategies/byte_stuffing/write.h

        include/BDSP/streams/strategies/cobs/mixins.h
        include/BDSP/streams/strategies/cobs/read.h
        include/BDSP/streams/strategies/cobs/types.h
        include/BDSP/streams/strategies/cobs/write.h
)

set(sources
        src/mixins.cpp
        src/receiver.cpp
        src/transmitter.cpp

        src/checksums/crc8.cpp

        src/streams/mixins.cpp
        src/streams/reader.cpp
        src/streams/writer.cpp

        src/streams/strategies/abstract/read.cpp
        src/streams/strategies/abstract/write.cpp

        src/streams/strategies/byte_stuffing/mixins.cpp
        src/streams/strategies/byte_stuffing/read.cpp
        src/streams/strategies/byte_stuffing/write.cpp

        src/streams/strategies/cobs/mixins.cpp
        src/streams/strategies/cobs/read.cpp
        src/streams/strategies/cobs/write.cpp
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

#----------------------------------------------------------------------------------------------------------------------
# BDSP target
#----------------------------------------------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

target_sources(BDSP PRIVATE ${sources})
target_compile_definitions(BDSP PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:BDSP_STATIC_DEFINE>")

target_include_directories(BDSP
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

set_target_properties(BDSP PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})

if (BDSP_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(cmake/BDSP-config.cmake.in BDSP-config.cmake
            INSTALL_DESTINATION "${BDSP_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(BDSP-config-version.cmake
            COMPATIBILITY SameMajorVersion)

    install(TARGETS BDSP EXPORT BDSP_export
            RUNTIME COMPONENT BDSP
            LIBRARY COMPONENT BDSP NAMELINK_COMPONENT BDSP-dev
            ARCHIVE COMPONENT BDSP-dev
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY include/
            TYPE INCLUDE
            COMPONENT BDSP-dev)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/BDSP/${export_file_name}"
            COMPONENT BDSP-dev
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/BDSP")

    set(targets_file "BDSP-shared-targets.cmake")

    if (NOT BUILD_SHARED_LIBS)
        set(targets_file "BDSP-static-targets.cmake")
    endif ()

    install(EXPORT BDSP_export
            COMPONENT BDSP-dev
            FILE "${targets_file}"
            DESTINATION "${BDSP_INSTALL_CMAKEDIR}"
            NAMESPACE BDSP::)

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/BDSP-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/BDSP-config-version.cmake"
            COMPONENT BDSP-dev
            DESTINATION "${BDSP_INSTALL_CMAKEDIR}")

    if (MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if (BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:BDSP>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else ()
            # TARGET_PDB_FILE does not work for pdb file generated for static library build, determining it manually
            set(pdb_file "$<TARGET_FILE_DIR:BDSP>/$<TARGET_FILE_PREFIX:BDSP>$<TARGET_FILE_BASE_NAME:BDSP>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif ()

        install(FILES "${pdb_file}"
                COMPONENT BDSP-dev
                CONFIGURATIONS Debug RelWithDebInfo
                DESTINATION "${pdb_file_destination}"
                OPTIONAL)
    endif ()
endif ()

#----------------------------------------------------------------------------------------------------------------------
# other targets
#----------------------------------------------------------------------------------------------------------------------

if (BDSP_BUILD_TESTS)
    message(STATUS "Install Google tests: ${INSTALL_GTEST}")
    enable_testing()
    add_subdirectory(tests)
endif ()

if (BDSP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (BDSP_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(docs include)
endif ()
