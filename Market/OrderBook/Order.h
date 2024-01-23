#pragma once

#include "IOrder.h"
#include <list>
#include <string>

class Order : public IOrder {
public:

    Order(std::string id, int quantity, double pricen, bool isBid);
    ~Order();

    inline bool operator<(const Order& other) const {
        return price < other.price;
    }

    int getQuantity() const override;
    std::string getId() const override;
    double getPrice() const override;
    bool getIsBid() const override;

    void setQuantity(int newQuantity) override;
    void setPrice(double newPrice) override;

private:
    double price;
    int quantity;
    std::string id;
    bool isBid;
};
