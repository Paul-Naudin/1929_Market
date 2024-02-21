# Market executable
add_executable(Market
    main.cpp
)
target_include_directories(Market PRIVATE OrderBook)
target_link_libraries(Market PRIVATE FIXProtocol Network)

# OrderBook library
add_library(OrderBookLib
    OrderBook/OrderBook.cpp
    OrderBook/Order.cpp
)
target_include_directories(OrderBookLib PRIVATE OrderBook)

# Unit tests
add_subdirectory(tests)
