# Orderbook lib
add_subdirectory(OrderBook)

# Market executable
add_executable(Market
    main.cpp
)
target_include_directories(Market PRIVATE OrderBook)
target_link_libraries(Market PRIVATE FIXProtocol Network OrderBookLib)


# Unit tests
add_subdirectory(tests)
