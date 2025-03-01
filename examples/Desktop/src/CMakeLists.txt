set(shared_sources utils.cpp)

set(EXAMPLES_PREFIX "")

foreach (item
        PPP_EXAMPLE
        COBS_EXAMPLE
        COBS_GSE_EXAMPLE
        COBS_ZPE_EXAMPLE
        BDSP_EXAMPLE
        REDUNDANCY_TESTER
)
    add_executable(${EXAMPLES_PREFIX}${item} ${item}.cpp)
    target_sources(${EXAMPLES_PREFIX}${item} PRIVATE ${shared_sources})
    target_link_libraries(${EXAMPLES_PREFIX}${item} PRIVATE BDSP::BDSP)
    if (NOT is_top_level)
        win_copy_deps_to_target_dir(${EXAMPLES_PREFIX}${item} BDSP::BDSP)
    endif ()
endforeach ()