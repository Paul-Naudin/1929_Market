#pragma once
#include <map>
#include "IOrderBook.h"
#include "Order.h"


class OrderBook : public IOrderBook {
public:
    OrderBook();
    ~OrderBook();

    bool addOrder(double price, int id, int quantity, bool isBid);
    bool removeOrder(int id);
    bool modifyOrder(int id, int newQuantity, int newPrice);

    bool execute();

    const std::multimap<double, Order> getBids() const override;
    const std::multimap<double, Order, std::greater<double>> getAsks() const override;
    
    void printOrderBook() const;

private:
    std::multimap<double, Order> bids;
    std::multimap<double, Order, std::greater<double>> asks;
};
