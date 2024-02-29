
#include "Order.h"
#include <string>

// Constructor
Order::Order(std::string id, int quantity, double price, OrderType type) : m_price(price), m_quantity(quantity), m_id(id), m_type(type) {}

// Destructor
Order::~Order() {}

// price getter
double Order::getPrice() const {
    return m_price;
}

// quantity getter
int Order::getQuantity() const {
    return m_quantity;
}

// id getter
std::string Order::getId() const {
    return m_id;
}

// OrderType getter
OrderType Order::getOrderType() const {
    return m_type;
}

// price setter
void Order::setPrice(double newPrice) {
    m_price = newPrice;
}

// quantity setter
void Order::setQuantity(int newQuantity) {
    m_quantity = newQuantity;
}
