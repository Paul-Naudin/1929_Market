#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/signalfd.h>

#include "OrderBook/OrderBook.h"
#include "Exchange.h"
#include "../Common/Network/network.hpp"
#include "../Common/FIXProtocol/FIXProtocol.hpp"

int main() {
//    try {
//        NetworkServer server;
//        server.addListeningSocket(1929);
//        server.run();
//    } catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//        return 1;
//    }

    Exchange exchange;

    OrderBook &usdEur = exchange.getOrderBook("USD/EUR");

    usdEur.printOrderBook();

    usdEur.addOrder(1, 100, OrderType::BID);
    usdEur.addOrder(1, 50, OrderType::ASK);
    usdEur.addOrder(2, 100, OrderType::BID);

    usdEur.printOrderBook();

    usdEur.execute();

    usdEur.printOrderBook();

    return 0;
}