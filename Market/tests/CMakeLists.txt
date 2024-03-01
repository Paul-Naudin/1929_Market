# Market tests
add_executable(MarketTests
    OrderTests.cpp
)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Link Google Test to the test executable
target_link_libraries(MarketTests gtest_main OrderBookLib)
target_include_directories(MarketTests PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_test(NAME MarketTests COMMAND MarketTests)