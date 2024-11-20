cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(CCTools VERSION 1.0.0 LANGUAGES CXX)
include(FetchContent)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add include directories
include_directories(include)

# Find and link external libraries
find_package(RatModels 2.017.7 REQUIRED)
find_package(Armadillo REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

# Find ZLIB
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()
find_package(ZLIB REQUIRED)

# Fetch jsoncpp
FetchContent_Declare(
    jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG        1.9.6 
)
# Disable jsoncpp tests and examples
set(JSONCPP_WITH_TESTS OFF CACHE INTERNAL "")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(jsoncpp)

# Get all source files in src directory
file(GLOB CCTOOLS_SOURCES "src/*.cpp")

# Create public library
add_library(CCTools STATIC ${CCTOOLS_SOURCES})
target_link_libraries(CCTools PUBLIC Rat::Models armadillo Boost::filesystem jsoncpp_static ZLIB::ZLIB)

# Specify include directories for consumers
target_include_directories(CCTools PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Determine if CCTools is the top-level project
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(CCTools_IS_TOPLEVEL ON)
else()
    set(CCTools_IS_TOPLEVEL OFF)
endif()

# Option to build tests and examples, default to ON if CCTools is top-level project
option(CCTools_BUILD_TESTS "Build CCTools tests and examples" ${CCTools_IS_TOPLEVEL})

# Conditional Block: Build Tests and Examples Only If CCTools_BUILD_TESTS is ON
if(CCTools_BUILD_TESTS)

    # Google Test integration
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Get all test files in test directory
    file(GLOB CCTOOLS_TEST_SOURCES "test/*.cpp")

    # Enable testing
    enable_testing()

    # Create the test executable
    add_executable(cctools_test ${CCTOOLS_SOURCES} ${CCTOOLS_TEST_SOURCES})
    target_link_libraries(cctools_test PRIVATE gtest_main CCTools)

    # Add tests
    add_test(NAME cctools_test COMMAND cctools_test)

    # Suppress DartConfiguration.tcl warnings
    set(CMAKE_SUPPRESS_REGENERATION true)

    # Disable CDash
    set(CTEST_USE_LAUNCHERS OFF)

    # Create the example executable
    add_executable(cctools_example examples/example.cpp)
    
    # Link the example executable with the CCTools library
    target_link_libraries(cctools_example PRIVATE CCTools)
    
    # Specify include directories for the example
    target_include_directories(cctools_example PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )

endif()
