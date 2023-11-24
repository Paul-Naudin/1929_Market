
#include <iostream>
#include "OrderBook.h"

// Constructor
OrderBook::OrderBook() {
    // Initialize the order book

    // TODO
}

// Destructor
OrderBook::~OrderBook() {
    // Clean up resources

    // TODO
}

// Add an order to the order book
bool OrderBook::addOrder(double price, int id, int quantity, bool isBid) {

    // TODO
    return false;
}

// Remove an order from the order book
bool OrderBook::removeOrder(int id) {
    // Implement the logic to remove an order from the order book
    // TODO
    return false;
}

// Modify an order in the order book
bool OrderBook::modifyOrder(int id, int newQuantity, int newPrice) {
    // Implement the logic to modify an order in the order book

    // TODO
    return false;
}

// Execute an order
bool OrderBook::execute() {
    // Implement the logic to execute an order

    // TODO
    return false;
}

// Get the bids from the order book
const std::multimap<double, Order> OrderBook::getBids() const {
    return this->bids;
}

// Get the asks from the order book
const std::multimap<double, Order, std::greater<double>> OrderBook::getAsks() const {
    return this->asks;
}

// Print the order book
void OrderBook::printOrderBook() const {
    std::cout << "Bids:" << std::endl;
    for (auto& bid : bids) {
        std::cout << bid.second.getNumber() << "\t" << bid.second.getQuantity() << "\t" << bid.second.getPrice() << std::endl;
    }
    std::cout << "Asks:" << std::endl;
    for (auto& ask : asks) {
        std::cout << ask.second.getNumber() << "\t" << ask.second.getQuantity() << "\t" << ask.second.getPrice() << std::endl;
    }
}