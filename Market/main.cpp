#include <iostream>
#include "OrderBook/OrderBook.h"

int main() {
    OrderBook orderBook;

    orderBook.addOrder(100, 10, true);
    orderBook.addOrder(100, 100, true);
    orderBook.addOrder(101, 50, true);
    orderBook.addOrder(100, 50, true);
    orderBook.addOrder(105, 50, false);

    orderBook.execute();

    orderBook.printOrderBook();

    orderBook.addOrder(105, 10, true);

    orderBook.printOrderBook();

    orderBook.execute();

    orderBook.printOrderBook();

    return 0;
}
