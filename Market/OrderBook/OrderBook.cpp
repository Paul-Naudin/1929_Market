
#include <iostream>
#include <algorithm>
#include "OrderBook.h"

// Constructor
OrderBook::OrderBook()
{
}

// Destructor
OrderBook::~OrderBook()
{
    // Delete all the Order objects
    for (auto &order : orders)
    {
        delete order.second;
    }
}

std::string OrderBook::addOrder(const std::string &id, const std::string &symbol, double price, int quantity, bool isBid)
{
    Order *order = new Order(id, symbol, quantity, price, isBid);
    orders[id] = order;

    if (isBid)
    {
        bids[price].push_back(order);
    }
    else
    {
        asks[price].push_back(order);
    }

    return id;
}

bool OrderBook::removeOrder(const std::string &id)
{
    auto it = orders.find(id);
    if (it != orders.end())
    {
        Order *order = it->second;
        orders.erase(it);

        if (order->getIsBid())
        {
            auto &bidDeque = bids[order->getPrice()];
            bidDeque.erase(std::remove(bidDeque.begin(), bidDeque.end(), order), bidDeque.end());
        }
        else
        {
            auto &askDeque = asks[order->getPrice()];
            askDeque.erase(std::remove(askDeque.begin(), askDeque.end(), order), askDeque.end());
        }

        delete order;
        return true;
    }
    return false;
}

bool OrderBook::modifyOrder(const std::string &id, int newQuantity, double newPrice)
{
    auto it = orders.find(id);
    if (it != orders.end())
    {
        Order *order = it->second;
        order->setQuantity(newQuantity);

        if (order->getIsBid())
        {
            auto &bidDeque = bids[order->getPrice()];
            bidDeque.erase(std::remove(bidDeque.begin(), bidDeque.end(), order), bidDeque.end());
            order->setPrice(newPrice);
            bids[newPrice].push_back(order);
        }
        else
        {
            auto &askDeque = asks[order->getPrice()];
            askDeque.erase(std::remove(askDeque.begin(), askDeque.end(), order), askDeque.end());
            order->setPrice(newPrice);
            asks[newPrice].push_back(order);
        }

        return true;
    }
    return false;
}

bool OrderBook::execute(double price, int quantity, bool isBid)
{
    if (bids.find(price) != bids.end() && asks.find(price) != asks.end())
    {
        auto &bidDeque = bids[price];
        auto &askDeque = asks[price];

        while (!bidDeque.empty() && !askDeque.empty() && quantity > 0)
        {
            Order *bidOrder = bidDeque.front();
            Order *askOrder = askDeque.front();

            int executionQuantity = std::min(bidOrder->getQuantity(), askOrder->getQuantity());
            bidOrder->setQuantity(bidOrder->getQuantity() - executionQuantity);
            askOrder->setQuantity(askOrder->getQuantity() - executionQuantity);
            quantity -= executionQuantity;

            if (bidOrder->getQuantity() == 0)
            {
                bidDeque.pop_front();
                delete bidOrder;
            }
            if (askOrder->getQuantity() == 0)
            {
                askDeque.pop_front();
                delete askOrder;
            }

            if (bidDeque.empty())
            {
                bids.erase(price);
            }

            if (askDeque.empty())
            {
                asks.erase(price);
            }
        }

        return true;
    }

    return false;
}

const std::map<double, std::deque<Order *>, std::greater<double>> &OrderBook::getBids() const
{
    return bids;
}

const std::map<double, std::deque<Order *>, std::greater<double>> &OrderBook::getAsks() const
{
    return asks;
}

void OrderBook::printOrderBook() const
{
    std::cout << "Liste des ordres dans le carnet d'ordres :" << std::endl;
    for (const auto &entry : bids)
    {
        double price = entry.first;
        const auto &ordersAtPrice = entry.second;
        for (const auto &order : ordersAtPrice)
        {
            std::cout << "Bid Order - Price: " << price << ", Quantity: " << order->getQuantity() << std::endl;
        }
    }

    for (const auto &entry : asks)
    {
        double price = entry.first;
        const auto &ordersAtPrice = entry.second;
        for (const auto &order : ordersAtPrice)
        {
            std::cout << "Ask Order - Price: " << price << ", Quantity: " << order->getQuantity() << std::endl;
        }
    }
}