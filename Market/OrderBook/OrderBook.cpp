
#include <iostream>
#include <algorithm>
#include "OrderBook.h"

// Constructor
OrderBook::OrderBook()
{
    // Initialize the order book

    // TODO
}

// Destructor
OrderBook::~OrderBook()
{
    // Clean up resources

    // TODO
}

// Add an order to the order book
bool OrderBook::addOrder(double price, int id, int quantity, bool isBid)
{
    // Check if the order already exists in the order book
    if (orders.find(id) != orders.end()) {
        return false;
    }

    // Create a new Order object
    Order *order = new Order(id, quantity, price, isBid);

    // Add the order to the order book
    orders.emplace(id, order);

    // Add the order to the bids or asks
    if (isBid) {
        bids[price].push_back(order);
    } else {
        asks[price].push_back(order);
    }

    return true;
}

// Remove an order from the order book
bool OrderBook::removeOrder(int id)
{
    // Find the order in the orders map
    auto it = orders.find(id);
    if (it == orders.end()) {
        return false;
    }

    // Get the pointer to the Order object
    Order *order = it->second;

    // Remove the order from the bids or asks map
    if (order->getIsBid()) {
        auto &deque = bids[order->getPrice()];
        deque.erase(std::remove(deque.begin(), deque.end(), order), deque.end());
        if (deque.empty()) {
            bids.erase(order->getPrice());
        }
    } else {
        auto &deque = asks[order->getPrice()];
        deque.erase(std::remove(deque.begin(), deque.end(), order), deque.end());
        if (deque.empty()) {
            asks.erase(order->getPrice());
        }
    }

    // Remove the order from the orders map
    orders.erase(it);

    // Delete the Order object
    delete order;

    return false;
}

// Modify an order in the order book
bool OrderBook::modifyOrder(int id, int newQuantity, int newPrice)
{
    // Find the order in the orders map
    auto it = orders.find(id);

    // Check if the order exists
    if (it == orders.end()) {
        return false;
    }

    // Get the pointer to the Order object
    Order *order = it->second;

    // Remove the order from the bids or asks map
    if (order->getIsBid()) {
        auto &deque = bids[order->getPrice()];
        deque.erase(std::remove(deque.begin(), deque.end(), order), deque.end());
        if (deque.empty()) {
            bids.erase(order->getPrice());
        }
    }
    else {
        auto &deque = asks[order->getPrice()];
        deque.erase(std::remove(deque.begin(), deque.end(), order), deque.end());
        if (deque.empty()) {
            asks.erase(order->getPrice());
        }
    }

    // Modify the order
    order->setPrice(newPrice);
    order->setQuantity(newQuantity);

    // Add the order to the bids or asks map
    if (order->getIsBid()) {
        bids[order->getPrice()].push_back(order);
    } else {
        asks[order->getPrice()].push_back(order);
    }

    return false;
}

// Execute an order
bool OrderBook::execute()
{
    // Implement the logic to execute an order

    // TODO
    return false;
}

// Get the bids from the order book
const std::map<double, std::deque<Order *>> OrderBook::getBids() const
{
    return this->bids;
}

// Get the asks from the order book
const std::map<double, std::deque<Order *>, std::greater<double>> OrderBook::getAsks() const
{
    return this->asks;
}

// Print the order book
void OrderBook::printOrderBook() const
{
    std::cout << "Bids:" << std::endl;
    for (const auto &bid : bids)
    {
        for (const auto &order : bid.second)
        {
            std::cout << order->getId() << "\t" << order->getQuantity() << "\t" << order->getPrice() << std::endl;
        }
    }
    std::cout << "Asks:" << std::endl;
    for (const auto &ask : asks)
    {
        for (const auto &order : ask.second)
        {
            std::cout << order->getId() << "\t" << order->getQuantity() << "\t" << order->getPrice() << std::endl;
        }
    }
}