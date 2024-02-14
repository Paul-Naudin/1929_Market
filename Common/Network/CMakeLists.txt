# Network library
add_library(Network
    network.cpp
)
target_include_directories(Network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})