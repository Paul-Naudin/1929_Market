cmake_minimum_required(VERSION 3.10)
project(1929_Market)

set(CMAKE_CXX_STANDARD 17)

# FIX Protocol library
add_library(FIXProtocol
    Common/FIXProtocol.cpp
)
target_include_directories(FIXProtocol PUBLIC FIXProtocol)

# Market executable
add_executable(Market
    Market/main.cpp
    Market/OrderBook/OrderBook.cpp
    Market/OrderBook/Order.cpp
)
target_include_directories(Market PRIVATE Market/OrderBook)
target_link_libraries(Market PRIVATE FIXProtocol)

# Client executable
add_executable(Client
    Client/main.cpp
)
target_include_directories(Client PRIVATE Client/Order)
target_link_libraries(Client PRIVATE FIXProtocol)