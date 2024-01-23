
#pragma once

#include <map>
#include "Order.h"

class IOrderBook {
public:
    virtual ~IOrderBook() = default;

    virtual std::string addOrder(double price, int quantity, bool isBid) = 0;
    virtual bool removeOrder(std::string id) = 0;
    virtual bool modifyOrder(std::string id, int newQuantity, int newPrice) = 0;

    virtual bool execute() = 0;

    virtual const std::map<double, std::deque<Order *>, std::greater<double>> getBids() const = 0;
    virtual const std::map<double, std::deque<Order *>> getAsks() const = 0;
};
