#include <gtest/gtest.h>
#include "../OrderBook/Order.h"

TEST(OrderTests, GettersAndSetters)
{
    // Create an Order object
    Order order("123", 10, 100.0, OrderType::BID);

    // Test the getters
    EXPECT_EQ(order.getPrice(), 100.0);
    EXPECT_EQ(order.getQuantity(), 10);
    EXPECT_EQ(order.getId(), "123");
    EXPECT_EQ(order.getOrderType(), OrderType::BID);

    // Test the setters
    order.setPrice(200.0);
    order.setQuantity(20);

    EXPECT_EQ(order.getPrice(), 200.0);
    EXPECT_EQ(order.getQuantity(), 20);
}
