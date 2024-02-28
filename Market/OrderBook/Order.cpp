
#include "Order.h"
#include <string>

// Constructor
Order::Order(std::string id, const std::string& symbol, int quantity, double price, bool isBid) : price(price), symbol(symbol), quantity(quantity), id(id), isBid(isBid) {}

// Destructor
Order::~Order() {}

// price getter
double Order::getPrice() const {
    return price;
}

const std::string& Order::getSymbol() const {
    return symbol;
}

// quantity getter
int Order::getQuantity() const {
    return quantity;
}

// id getter
std::string Order::getId() const {
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
