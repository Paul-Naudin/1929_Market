#include "Exchange.h"

Exchange::Exchange()
{
    m_orderBooks["USD/EUR"] = OrderBook();
    m_orderBooks["EUR/USD"] = OrderBook();
    m_orderBooks["USD/JPY"] = OrderBook();
}

Exchange::~Exchange()
{
}