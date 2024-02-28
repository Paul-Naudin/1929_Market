
#pragma once

#include <map>
#include <deque>
#include "Order.h"

class IOrderBook
{
public:
    virtual ~IOrderBook() = default;

    virtual std::string addOrder(const std::string &id, const std::string &symbol, double price, int quantity, bool isBid) = 0;
    virtual bool removeOrder(const std::string &id) = 0;
    virtual bool modifyOrder(const std::string &id, int newQuantity, double newPrice) = 0;

    virtual bool execute(double price, int quantity, bool isBid) = 0;

    virtual const std::map<double, std::deque<Order *>, std::greater<double>> &getBids() const = 0;
    virtual const std::map<double, std::deque<Order *>, std::greater<double>> &getAsks() const = 0;
};
