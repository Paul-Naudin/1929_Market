#include <gtest/gtest.h>
#include "../OrderBook/Order.h"

TEST(OrderTests, GettersAndSetters)
{
    // Create an Order object
    Order order(std::string("123"), "SYMBOL", 10, 100.0, true);

    // Test the getters
    EXPECT_EQ(order.getPrice(), 100.0);
    EXPECT_EQ(order.getQuantity(), 10);
    EXPECT_EQ(order.getId(), "123");
    EXPECT_EQ(order.getIsBid(), true);

    // Test the setters
    order.setPrice(200.0);
    order.setQuantity(20);

    EXPECT_EQ(order.getPrice(), 200.0);
    EXPECT_EQ(order.getQuantity(), 20);
}
