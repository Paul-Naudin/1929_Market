
#include <iostream>
#include <algorithm>
#include "OrderBook.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// Constructor
OrderBook::OrderBook()
{
}

// Destructor
OrderBook::~OrderBook()
{
    // Delete all the Order objects
    for (auto &order : orders) {
        delete order.second;
    }
}

// Add an order to the order book
const std::string OrderBook::addOrder(double price, int quantity, OrderType type)
{
    std::string id = generateId();

    // Check if the order already exists in the order book
    if (orders.find(id) != orders.end()) {
        return nullptr;
    }

    // Create a new Order object
    Order *order = new Order(id, quantity, price, type);

    // Add the order to the order book
    orders.emplace(id, order);

    // Add the order to the bids or asks
    if (type == OrderType::BID)
        bids[price].push_back(order);
    else
        asks[price].push_back(order);

    return id;
}

// Remove an order from the order book
bool OrderBook::removeOrder(const std::string &id)
{
    // Find the order in the orders map
    auto it = orders.find(id);
    if (it == orders.end()) {
        return false;
    }

    // Get the pointer to the Order object
    Order *order = it->second;

    // Remove the order from the bids or asks map
    if (order->getOrderType() == OrderType::BID) {
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
bool OrderBook::modifyOrder(const std::string &id, int newQuantity, int newPrice)
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
    if (order->getOrderType() == OrderType::BID) {
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

    // Modify the order
    order->setPrice(newPrice);
    order->setQuantity(newQuantity);

    // Add the order to the bids or asks map
    if (order->getOrderType() == OrderType::BID) {
        bids[order->getPrice()].push_back(order);
    } else {
        asks[order->getPrice()].push_back(order);
    }

    return false;
}

// Execute an order
bool OrderBook::execute()
{
    // Check if there are any bids or asks
    if (bids.empty() || asks.empty()) {
        return false;
    }

    // Get the best bid and ask
    auto bestBid = bids.begin();
    auto bestAsk = asks.begin();

    // Continue matching orders until no more matches can be made
    while (bestBid->first >= bestAsk->first) {
        // Get the best bid and ask orders
        Order *bestBidOrder = bestBid->second.front();
        Order *bestAskOrder = bestAsk->second.front();

        // Check if the best bid quantity is greater than the best ask quantity
        if (bestBidOrder->getQuantity() > bestAskOrder->getQuantity()) {
            // Modify the best bid order
            bestBidOrder->setQuantity(bestBidOrder->getQuantity() - bestAskOrder->getQuantity());
            // Remove the best ask order
            removeOrder(bestAskOrder->getId());
        } else if (bestBidOrder->getQuantity() < bestAskOrder->getQuantity()) {
            // Modify the best ask order
            bestAskOrder->setQuantity(bestAskOrder->getQuantity() - bestBidOrder->getQuantity());
            // Remove the best bid order
            removeOrder(bestBidOrder->getId());
        } else {
            // Remove the best bid and ask orders
            removeOrder(bestBidOrder->getId());
            removeOrder(bestAskOrder->getId());
        }

        // Update the best bid and ask
        if (!bids.empty()) {
            bestBid = bids.begin();
        }
        if (!asks.empty()) {
            bestAsk = asks.begin();
        } else {
            break;
        }
    }

    return true;
}

// Get the bids from the order book
const std::map<double, std::deque<Order *>, std::greater<double>> OrderBook::getBids() const
{
    return this->bids;
}

// Get the asks from the order book
const std::map<double, std::deque<Order *>> OrderBook::getAsks() const
{
    return this->asks;
}

// Print the order book
void OrderBook::printOrderBook() const
{
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Bids:" << std::endl;
    for (const auto &bid : bids) {
        std::cout << bid.first << std::endl;
        for (const auto &order : bid.second) {
            std::cout << "\t" << order->getId() << "\t" << order->getQuantity() << "\t" << std::endl;
        }
    }
    std::cout << "Asks:" << std::endl;
    for (const auto &ask : asks) {
        std::cout << ask.first << std::endl;
        for (const auto &order : ask.second) {
            std::cout << "\t" << order->getId() << "\t" << order->getQuantity() << "\t" << std::endl;
        }
    }
}

// Generate a unique id for an order
std::string OrderBook::generateId()
{
    static boost::uuids::random_generator generator;
    boost::uuids::uuid uuid = generator();
    return boost::uuids::to_string(uuid);
}
