#pragma once

class IOrder {
public:
    virtual ~IOrder() = default;

    virtual int getNumber() const = 0;
    virtual int getQuantity() const = 0;
    virtual double getPrice() const = 0;
};
