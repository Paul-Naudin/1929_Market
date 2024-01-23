#pragma once
#include <map>
#include <unordered_map>
#include <deque>
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

    const std::map<double, std::deque<Order *>> getBids() const override;
    const std::map<double, std::deque<Order *>, std::greater<double>> getAsks() const override;
    
    void printOrderBook() const;

private:
    std::unordered_map<int, Order *> orders;
    std::map<double, std::deque<Order *>> bids;
    std::map<double, std::deque<Order *>, std::greater<double>> asks;
};
