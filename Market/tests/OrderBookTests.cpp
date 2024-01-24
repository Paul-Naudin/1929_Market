#include <gtest/gtest.h>
#include "../OrderBook/OrderBook.h"

TEST(OrderBookTests, InstanceExists) {
    OrderBook orderBook;

    EXPECT_TRUE(&orderBook != nullptr);
}

TEST(OrderBookTests, AddOrder) {
    OrderBook orderBook;

    std::string id = orderBook.addOrder(100.0, 10, true);

    EXPECT_TRUE(id != "");
    EXPECT_TRUE(orderBook.getBids().size() == 1);
    EXPECT_FALSE(orderBook.getAsks().size() == 1);

    EXPECT_EQ(id, orderBook.getBids().begin()->second.front()->getId());
}

TEST(OrderBookTests, RemoveOrder) {
    OrderBook orderBook;

    std::string id = orderBook.addOrder(100.0, 10, true);

    EXPECT_TRUE(orderBook.removeOrder(id));
    EXPECT_FALSE(orderBook.removeOrder(id));
    EXPECT_TRUE(orderBook.getBids().size() == 0);
    EXPECT_TRUE(orderBook.getAsks().size() == 0);
}

TEST(OrderBookTests, ModifyOrder) {
    OrderBook orderBook;

    std::string id = orderBook.addOrder(100.0, 10, true);
    std::string id2 = orderBook.addOrder(100.0, 10, false);

    EXPECT_TRUE(orderBook.modifyOrder(id, 20, 200.0));
    EXPECT_TRUE(orderBook.getBids().size() == 1);
    EXPECT_TRUE(orderBook.getAsks().size() == 1);

    EXPECT_TRUE(orderBook.modifyOrder(id2, 20, 200.0));

    EXPECT_EQ(200.0, orderBook.getBids().begin()->first);
    EXPECT_EQ(20, orderBook.getBids().begin()->second.front()->getQuantity());

    orderBook.removeOrder(id);

    EXPECT_FALSE(orderBook.modifyOrder(id, 20, 200.0));
}

TEST(OrderBookTests, ExecuteNoBidsOrAsks) {
    OrderBook orderBook;
    EXPECT_FALSE(orderBook.execute());
}

TEST(OrderBookTests, ExecuteBidQuantityGreaterThanAskQuantity) {
    OrderBook orderBook;
    orderBook.addOrder(100.0, 10, true);  // Bid
    orderBook.addOrder(100.0, 5, false);  // Ask
    EXPECT_TRUE(orderBook.execute());
    // Check that the remaining bid quantity is 5
    EXPECT_EQ(orderBook.getBids().begin()->second.front()->getQuantity(), 5);
}

TEST(OrderBookTests, ExecuteBidQuantityLessThanAskQuantity) {
    OrderBook orderBook;
    orderBook.addOrder(100.0, 5, true);   // Bid
    orderBook.addOrder(100.0, 10, false); // Ask
    EXPECT_TRUE(orderBook.execute());
    // Check that the remaining ask quantity is 5
    EXPECT_EQ(orderBook.getAsks().begin()->second.front()->getQuantity(), 5);
}

TEST(OrderBookTests, ExecuteBidQuantityEqualToAskQuantity) {
    OrderBook orderBook;
    orderBook.addOrder(100.0, 10, true);  // Bid
    orderBook.addOrder(100.0, 10, false); // Ask
    EXPECT_TRUE(orderBook.execute());
    // Check that there are no remaining bids or asks
    EXPECT_TRUE(orderBook.getBids().empty());
    EXPECT_TRUE(orderBook.getAsks().empty());
}

TEST(OrderBookTests, PrintOrderBook) {
    OrderBook orderBook;

    std::string id1 = orderBook.addOrder(100.0, 10, true);
    std::string id2 = orderBook.addOrder(100.0, 10, false);

    testing::internal::CaptureStdout();
    orderBook.printOrderBook();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output != "");
    EXPECT_EQ(output, "----------------------------------------\nBids:\n100\n\t" + id1 + "\t10\t\nAsks:\n100\n\t" + id2 + "\t10\t\n");
}

TEST(OrderBookTests, GenerateId) {
    std::string id = OrderBook::generateId();

    EXPECT_TRUE(id != "");

    for (int i = 0; i < 1000; i++) {
        std::string newId = OrderBook::generateId();

        EXPECT_TRUE(newId != "");
        EXPECT_FALSE(newId == id);

        id = newId;
    }
}