#include "Exchange.h"

Exchange::Exchange()
{
    m_orderBooks["AAPL"] = OrderBook();
}

Exchange::~Exchange()
{
}