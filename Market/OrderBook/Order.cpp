
#include "Order.h"

// Constructor
Order::Order(double price) : price(price) {}

// Destructor
Order::~Order() {}

// number of orders getter
int Order::getNumber() const {
    return dataList.size();
}

// price getter
double Order::getPrice() const {
    return price;
}

// quantity getter
int Order::getQuantity() const {
    int totalQuantity = 0;
    for (auto& data : dataList) {
        totalQuantity += data.quantity;
    }
    return totalQuantity;
}
