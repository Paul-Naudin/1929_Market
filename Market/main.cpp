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

    std::string id1 = usdEur.addOrder(1, 100, OrderType::BID);
    std::string id2 = usdEur.addOrder(1, 50, OrderType::ASK);
    usdEur.addOrder(2, 100, OrderType::BID);

    usdEur.printOrderBook();

    usdEur.execute();

    usdEur.printOrderBook();

    std::cout << id1 << usdEur.removeOrder(id1) << std::endl;
    std::cout << id2 << usdEur.removeOrder(id2) << std::endl;

    std::cout << usdEur.modifyOrder(id1, 1, 100) << std::endl;

    usdEur.printOrderBook();

    usdEur.execute();
    usdEur.execute();
    usdEur.execute();
    usdEur.execute();
    usdEur.execute();

    return 0;
}