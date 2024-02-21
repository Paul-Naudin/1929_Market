#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/signalfd.h>

#include "OrderBook/OrderBook.h"
#include "../Common/Network/network.hpp"
#include "../Common/FIXProtocol/FIXProtocol.hpp"

int main() {
    try {
        NetworkServer server;
        server.addListeningSocket(1929);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}