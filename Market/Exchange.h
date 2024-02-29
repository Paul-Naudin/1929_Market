#include "OrderBook.h"

class Exchange {
public:
    Exchange();
    ~Exchange();

    OrderBook &getOrderBook(const std::string &symbol)
    {
        return m_orderBooks[symbol];
    }
protected:
    std::map<std::string, OrderBook> m_orderBooks;
};