# Network library
add_library(Network
    network.cpp
)
target_include_directories(Network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Network PUBLIC FIXProtocol)

include_directories(${CMAKE_SOURCE_DIR}/Market/OrderBook)
target_link_libraries(Network PRIVATE OrderBookLib)