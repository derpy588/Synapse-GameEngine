# ======== BUILDING MAIN LIBRARY ========
cmake_minimum_required(VERSION 3.10)
project(SynapseEngine VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../Packaging")

if (MSVC)
    add_compile_options(
        $<$<CONFIG:Debug>:/MDd>  # Use dynamic debug runtime in Debug mode
        $<$<CONFIG:Release>:/MD> # Use dynamic release runtime in Release mode
    )
endif()

# Build SynapseEngine as a static library
add_library(SynapseEngine STATIC
    src/engine.cpp
    
    # Add other source files here
)

# Specify include directories for the library
target_include_directories(SynapseEngine PUBLIC include)

# Link dependencies (e.g., SDL2, OpenGL)
find_package(SDL2 REQUIRED)
target_link_libraries(SynapseEngine PUBLIC SDL2::SDL2)

find_package(OpenGL REQUIRED)
target_link_libraries(SynapseEngine PUBLIC OpenGL::GL)

# ======== BUILDING TESTS ========
if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# Set up Google Test for testing
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(tests tests/test_main.cpp)
target_link_libraries(tests PRIVATE SynapseEngine gtest_main)

# Enable CTest
enable_testing()
add_test(NAME EngineTests COMMAND tests)

# Optionally build examples
#add_subdirectory(examples/example1)
#add_subdirectory(examples/example2)

# ======== PACKAG LIBRARY ========

# Install the library (assuming your target is named "SynapseEngine")
install(TARGETS SynapseEngine DESTINATION lib)

# Install public headers
install(DIRECTORY include/SynapseEngine DESTINATION include)

# Install examples (optional)
install(DIRECTORY examples/ DESTINATION examples)

# Install docs
install(DIRECTORY docs/ DESTINATION docs)

# Set package metadata
set(CPACK_PACKAGE_NAME "SynapseEngine")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION "A game engine library")
set(CPACK_GENERATOR "ZIP;TGZ")  # Creates .zip and .tar.gz files

# Include CPack
include(CPack)