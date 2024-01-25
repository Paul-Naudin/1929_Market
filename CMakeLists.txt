cmake_minimum_required(VERSION 3.10)
project(1929_Market)

set(CMAKE_CXX_STANDARD 17)

# Set flags for gcov
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

# Enable testing
enable_testing()

# Add sub-projects
add_subdirectory(Common)
add_subdirectory(Client)
add_subdirectory(Market)