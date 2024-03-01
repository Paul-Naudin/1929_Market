#pragma once

#include <string>

enum class OrderType {
    BID,
    ASK
};

class IOrder {
public:
    virtual ~IOrder() = default;

    virtual int getQuantity() const = 0;
    virtual const std::string& getId() const = 0;
    virtual double getPrice() const = 0;
    virtual OrderType getOrderType() const = 0;

    virtual void setQuantity(int newQuantity) = 0;
    virtual void setPrice(double newPrice) = 0;
};
