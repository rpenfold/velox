# Test configuration
include(FetchContent)

# Fetch Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test executable
file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(xl-formula-tests ${TEST_SOURCES})

target_link_libraries(xl-formula-tests 
    xl-formula
    gtest_main
    gmock_main
)

target_include_directories(xl-formula-tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/cpp/include
    ${CMAKE_SOURCE_DIR}/cpp
)

# Discover tests for CTest
include(GoogleTest)
gtest_discover_tests(xl-formula-tests)

# Coverage target (optional)
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(xl-formula-tests PRIVATE --coverage)
        target_link_options(xl-formula-tests PRIVATE --coverage)
        
        # Add coverage target
        find_program(GCOVR_PATH gcovr)
        if(GCOVR_PATH)
            add_custom_target(coverage
                COMMAND ${GCOVR_PATH} 
                    --root ${CMAKE_SOURCE_DIR}
                    --filter ${CMAKE_SOURCE_DIR}/cpp/
                    --filter ${CMAKE_SOURCE_DIR}/cpp/include/
                    --html --html-details 
                    --output coverage.html
                    --print-summary
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating coverage report"
            )
            add_dependencies(coverage xl-formula-tests)
        endif()
    endif()
endif()