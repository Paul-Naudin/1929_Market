cmake_minimum_required(VERSION 3.10)
project(1929_Market)

set(CMAKE_CXX_STANDARD 17)

# Enable testing
enable_testing()

# Add sub-projects
add_subdirectory(Common)
add_subdirectory(Client)
add_subdirectory(Market)

# Add tests
add_subdirectory(Market/tests)