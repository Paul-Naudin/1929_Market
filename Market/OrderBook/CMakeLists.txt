# OrderBook library
add_library(OrderBookLib
    OrderBook.cpp
    Order.cpp
    Exchange.cpp
)
target_include_directories(OrderBookLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
