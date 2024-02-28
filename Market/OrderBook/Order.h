#pragma once

#include "IOrder.h"
#include <list>
#include <string>

class Order
{
public:
    Order(std::string id, const std::string &symbol, int quantity, double pricen, bool isBid);
    ~Order();

    inline bool operator<(const Order &other) const
    {
        return price < other.price;
    }

    int getQuantity() const;
    const std::string &getSymbol() const;
    std::string getId() const;
    double getPrice() const;
    bool getIsBid() const;

    void setQuantity(int newQuantity);
    void setPrice(double newPrice);

private:
    double price;
    int quantity;
    std::string id;
    const std::string &symbol;
    bool isBid;
};
