cmake_minimum_required(VERSION 3.10)
project(SynapseEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your source files
add_executable(SynapseEngine src/main.cpp)

# Find and link SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(SynapseEngine PRIVATE SDL2::SDL2)

# Find and link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(SynapseEngine PRIVATE OpenGL::GL)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(tests tests/test_main.cpp)
target_link_libraries(tests PRIVATE gtest_main)