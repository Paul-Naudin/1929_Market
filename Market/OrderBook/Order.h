#pragma once

#include "IOrder.h"
#include <list>

class Order : public IOrder {
public:
    struct Data {
        int quantity;
        int id;
    };

    Order(double price);
    ~Order();

    inline bool operator<(const Order& other) const {
        return price < other.price;
    }

    int getNumber() const override;
    int getQuantity() const override;
    double getPrice() const override;

private:
    double price;
    std::list<Data> dataList;
};
