#pragma once
#include <map>
#include <unordered_map>
#include <deque>
#include <string>
#include "IOrderBook.h"
#include "Order.h"
#include "FIXProtocol.hpp"

class OrderBook
{
public:
    OrderBook();

    std::string addOrder(const std::string &id, const std::string &symbol, double price, int quantity, bool isBid);
    bool removeOrder(const std::string &id);
    bool modifyOrder(const std::string &id, int newQuantity, double newPrice);

    bool execute(double price, int quantity, bool isBid);

    const std::map<double, std::deque<Order *>, std::greater<double>> &getBids() const;
    const std::map<double, std::deque<Order *>, std::greater<double>> &getAsks() const;

    void printOrderBook() const;

private:
    // Structure de données pour stocker les ordres
    std::map<std::string, Order *> orders;
    std::map<double, std::deque<Order *>, std::greater<double>> bids;
    std::map<double, std::deque<Order *>, std::greater<double>> asks;
};

class Exchange
{
private:
    std::unordered_map<std::string, OrderBook> orderBooks; // Stocke les carnets d'ordres par instrument

public:
    void processNewOrder(const Order &order)
    {
        std::string symbol = order.getSymbol();
        if (orderBooks.find(symbol) == orderBooks.end())
        {
            orderBooks.emplace(symbol, OrderBook());
        }

        OrderBook &orderBook = orderBooks.at(symbol);
        orderBook.addOrder(order.getId(), order.getSymbol(), order.getPrice(), order.getQuantity(), (order.getIsBid() == '1'));
    }

    void manageOrderInteraction(OrderBook &orderBook)
    {
        for (auto it = orderBook.getBids().begin(); it != orderBook.getBids().end(); ++it)
        {
            double price = it->first;
            int quantity = it->second.front()->getQuantity();
            orderBook.execute(price, quantity, true);
        }

        for (auto it = orderBook.getAsks().begin(); it != orderBook.getAsks().end(); ++it)
        {
            double price = it->first;
            int quantity = it->second.front()->getQuantity();
            orderBook.execute(price, quantity, false);
        }
    }

    OrderBook &getOrderBook(const std::string &symbol)
    {
        return orderBooks.at(symbol);
    }
};