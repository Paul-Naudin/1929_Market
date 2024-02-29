#pragma once

#include "IOrder.h"
#include <list>
#include <string>


class Order : public IOrder {
public:

    Order(std::string id, int quantity, double pricen, OrderType type);
    ~Order();

    inline bool operator<(const Order& other) const {
        return m_price < other.m_price;
    }

    int getQuantity() const override;
    std::string getId() const override;
    double getPrice() const override;
    OrderType getOrderType() const override;

    void setQuantity(int newQuantity) override;
    void setPrice(double newPrice) override;

private:
    double m_price;
    int m_quantity;
    std::string m_id;
    OrderType m_type;
};
