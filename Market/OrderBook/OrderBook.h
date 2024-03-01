#pragma once
#include <map>
#include <unordered_map>
#include <deque>
#include <string>
#include "IOrderBook.h"
#include "Order.h"
// #include <boost/uuid/uuid_generators.hpp>
// #include <boost/uuid/uuid_io.hpp>


class OrderBook : public IOrderBook {
public:
    OrderBook();
    ~OrderBook();

    const std::string addOrder(double price, int quantity, OrderType type) override;
    bool removeOrder(const std::string &id) override;
    bool modifyOrder(const std::string &id, int newQuantity, int newPrice) override;

    bool execute();

    const std::map<double, std::deque<Order *>, std::greater<double>> getBids() const override;
    const std::map<double, std::deque<Order *>> getAsks() const override;
    
    void printOrderBook() const;

    static std::string generateId();

private:
    std::unordered_map<std::string, Order *> orders;
    std::map<double, std::deque<Order *>, std::greater<double>> bids;
    std::map<double, std::deque<Order *>> asks;
};
