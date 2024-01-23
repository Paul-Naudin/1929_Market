
#pragma once

#include <map>
#include "Order.h"

class IOrderBook {
public:
    virtual ~IOrderBook() = default;

    virtual bool addOrder(double price, int id, int quantity, bool isBid) = 0;
    virtual bool removeOrder(int id) = 0;
    virtual bool modifyOrder(int id, int newQuantity, int newPrice) = 0;

    virtual bool execute() = 0;

    virtual const std::map<double, std::deque<Order *>> getBids() const = 0;
    virtual const std::map<double, std::deque<Order *>, std::greater<double>> getAsks() const = 0;
};
