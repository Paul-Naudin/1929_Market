#include <gtest/gtest.h>
#include "../OrderBook/Order.h"


TEST(OrderTests, GettersAndSetters) {
    // Create an Order object
    Order order("123", 10, 100.0, true);

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

TEST(OrderTests, LessThanOperator) {
    // Create two Order objects
    Order order1("123", 10, 100.0, true);
    Order order2("124", 20, 200.0, true);

    // Test the < operator
    EXPECT_TRUE(order1 < order2);
}
