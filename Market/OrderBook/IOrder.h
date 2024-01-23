#pragma once

class IOrder {
public:
    virtual ~IOrder() = default;

    virtual int getQuantity() const = 0;
    virtual int getId() const = 0;
    virtual double getPrice() const = 0;
    virtual bool getIsBid() const = 0;

    virtual void setQuantity(int newQuantity) = 0;
    virtual void setPrice(double newPrice) = 0;
};
