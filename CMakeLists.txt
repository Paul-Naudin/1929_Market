cmake_minimum_required(VERSION 3.10)
project(Market)

set(CMAKE_CXX_STANDARD 14)

add_executable(Market
    Market/main.cpp
    Market/OrderBook/OrderBook.cpp
    Market/OrderBook/Order.cpp
)

target_include_directories(Market PRIVATE Market/OrderBook)