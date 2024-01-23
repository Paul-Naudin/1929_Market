
#include "Order.h"

// Constructor
Order::Order(int id, int quantity, double price, bool isBid) : price(price), quantity(quantity), id(id), isBid(isBid) {}

// Destructor
Order::~Order() {}

// price getter
double Order::getPrice() const {
    return price;
}

// quantity getter
int Order::getQuantity() const {
    return quantity;
}

// id getter
int Order::getId() const {
    return id;
}

// isBid getter
bool Order::getIsBid() const {
    return isBid;
}

// price setter
void Order::setPrice(double newPrice) {
    price = newPrice;
}

// quantity setter
void Order::setQuantity(int newQuantity) {
    quantity = newQuantity;
}
