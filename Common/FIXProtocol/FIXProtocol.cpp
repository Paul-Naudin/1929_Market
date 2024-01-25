#include "FIXProtocol.hpp"
#include <vector>

void splitString(const std::string &input, char delimiter, std::vector<std::string> &result)
{
    std::istringstream stream(input);
    std::string token;
    while (std::getline(stream, token, delimiter))
    {
        result.push_back(token);
    }
}

namespace FIXProtocol
{
    const std::string &FIXMessage::getBeginString() const
    {
        return beginString;
    }

    int FIXMessage::getBodyLength() const
    {
        return bodyLength;
    }

    void FIXMessage::setBodyLength(int length)
    {
        bodyLength = length;
    }

    const std::string &FIXMessage::getMsgType() const
    {
        return msgType;
    }

    void FIXMessage::setMsgType(const std::string &type)
    {
        msgType = type;
    }

    const std::string &FIXMessage::getSenderCompID() const
    {
        return senderCompID;
    }

    void FIXMessage::setSenderCompID(const std::string &value)
    {
        senderCompID = value;
    }

    const std::string &FIXMessage::getTargetCompID() const
    {
        return targetCompID;
    }

    void FIXMessage::setTargetCompID(const std::string &value)
    {
        targetCompID = value;
    }

    int FIXMessage::getMsgSeqNum() const
    {
        return msgSeqNum;
    }

    void FIXMessage::setMsgSeqNum(int value)
    {
        msgSeqNum += value;
    }

    const std::string &FIXMessage::getSendingTime() const
    {
        return sendingTime;
    }

    void FIXMessage::setSendingTime()
    {
        auto now = std::chrono::system_clock::now();
        auto nowTimeT = std::chrono::system_clock::to_time_t(now);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&nowTimeT), "%Y%m%d-%H:%M:%S");

        // Add milliseconds with ".SSS" format
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        oss << "." << std::setfill('0') << std::setw(3) << milliseconds.count();

        sendingTime = oss.str();
    }

    void FIXMessage::setSendingTime(std::string value)
    {
        sendingTime = value;
    }

    int FIXMessage::getCheckSum() const
    {
        return checkSum;
    }

    void FIXMessage::setCheckSum(const std::string &message)
    {
        const unsigned char *byteData = reinterpret_cast<const unsigned char *>(message.c_str());

        int sum = 0;
        for (size_t i = 0; i < message.length(); ++i)
        {
            sum += static_cast<int>(byteData[i]);
        }

        checkSum = sum % 256;
    }

    std::string FIXMessage::formatHeader() const
    {
        std::ostringstream oss;
        oss << "8=" << beginString << SOH
            << "9=" << bodyLength << SOH
            << "35=" << msgType << SOH
            << "49=" << senderCompID << SOH
            << "56=" << targetCompID << SOH
            << "34=" << msgSeqNum << SOH
            << "52=" << sendingTime << SOH;
        return oss.str();
    }

    std::string FIXMessage::formatTrailer(const std::string &message)
    {
        std::ostringstream oss;
        setCheckSum(message);
        oss << "10=" << getCheckSum() << SOH;
        return oss.str();
    }

    // Derived classes

    // LOGON
    const std::string LogonMessage::serialize(std::string sendCompID,
                                              std::string targetCompID, int bodyLength)
    {
        setMsgType("A");
        setSenderCompID(sendCompID);
        setTargetCompID(targetCompID);
        setMsgSeqNum();
        setSendingTime();
        setBodyLength(bodyLength);

        std::ostringstream oss;

        // Header
        oss << formatHeader();

        // Body (Logon)

        oss << "98=" << encryptMethod << SOH
            << "108=" << heartBtInt << SOH;

        if (username != "")
            oss << "553=" << username << SOH;

        if (password != "")
            oss << "554=" << password << SOH;

        // Trailer
        oss << formatTrailer(oss.str());

        return oss.str();
    }

    const std::string &LogonMessage::getEncryptMethod() const
    {
        return encryptMethod;
    }

    int LogonMessage::getHeartBtInt() const
    {
        return heartBtInt;
    }

    void LogonMessage::setHeartBtInt(int value)
    {
        heartBtInt = value;
    }

    const std::string &LogonMessage::getUsername() const
    {
        return username;
    }

    void LogonMessage::setUsername(const std::string &value)
    {
        username = value;
    }

    const std::string &LogonMessage::getPassword() const
    {
        return password;
    }

    void LogonMessage::setPassword(const std::string &value)
    {
        password = value;
    }

    // NEW ORDER
    std::string NewOrderMessage::serialize(std::string sendCompID, std::string targetCompID, int bodyLength)
    {
        setMsgType("D");
        setSenderCompID(sendCompID);
        setTargetCompID(targetCompID);
        setMsgSeqNum();
        setSendingTime();
        setBodyLength(bodyLength);

        std::ostringstream oss;

        oss << formatHeader();

        // Body
        oss << "11=" << clOrdID << SOH
            << "21=" << handlInst << SOH
            << "55=" << symbol << SOH
            << "54=" << side << SOH
            << "60=" << transactTime << SOH
            << "40=" << ordType << SOH;

        oss << formatTrailer(oss.str());

        return oss.str();
    }

    // Getters and setters
    const std::string &NewOrderMessage::getClOrdID() const
    {
        return clOrdID;
    }

    void NewOrderMessage::setClOrdID(const std::string &value)
    {
        clOrdID = value;
    }

    char NewOrderMessage::getHandlInst() const
    {
        return handlInst;
    }

    void NewOrderMessage::setHandlInst(char value)
    {
        handlInst = value;
    }

    const std::string &NewOrderMessage::getSymbol() const
    {
        return symbol;
    }

    void NewOrderMessage::setSymbol(const std::string &value)
    {
        symbol = value;
    }

    char NewOrderMessage::getSide() const
    {
        return side;
    }

    void NewOrderMessage::setSide(char value)
    {
        side = value;
    }

    const std::string &NewOrderMessage::getTransactTime() const
    {
        return transactTime;
    }

    void NewOrderMessage::setTransactTime(const std::string &value)
    {
        transactTime = value;
    }

    char NewOrderMessage::getOrdType() const
    {
        return ordType;
    }

    void NewOrderMessage::setOrdType(char value)
    {
        ordType = value;
    }

    // ORDER CANCEL / REPLACE REQUEST

    std::string OrderCancelReplaceRequest::serialize(std::string sendCompID, std::string targetCompID, int bodyLength)
    {
        setMsgType("G");
        setSenderCompID(sendCompID);
        setTargetCompID(targetCompID);
        setMsgSeqNum();
        setSendingTime();
        setBodyLength(bodyLength);

        std::ostringstream oss;

        oss << formatHeader();

        // Body
        oss << "41=" << origClOrdID << SOH
            << "11=" << clOrdID << SOH
            << "21=" << handlInst << SOH
            << "55=" << symbol << SOH
            << "54=" << side << SOH
            << "60=" << transactTime << SOH
            << "40=" << ordType << SOH;

        oss << formatTrailer(oss.str());

        return oss.str();
    }

    // Getters and Setters
    const std::string &OrderCancelReplaceRequest::getOrigClOrdID() const
    {
        return origClOrdID;
    }

    void OrderCancelReplaceRequest::setOrigClOrdID(const std::string &value)
    {
        origClOrdID = value;
    }

    const std::string &OrderCancelReplaceRequest::getClOrdID() const
    {
        return clOrdID;
    }

    void OrderCancelReplaceRequest::setClOrdID(const std::string &value)
    {
        clOrdID = value;
    }

    char OrderCancelReplaceRequest::getHandlInst() const
    {
        return handlInst;
    }

    void OrderCancelReplaceRequest::setHandlInst(char value)
    {
        handlInst = value;
    }

    const std::string &OrderCancelReplaceRequest::getSymbol() const
    {
        return symbol;
    }

    void OrderCancelReplaceRequest::setSymbol(const std::string &value)
    {
        symbol = value;
    }

    char OrderCancelReplaceRequest::getSide() const
    {
        return side;
    }

    void OrderCancelReplaceRequest::setSide(char value)
    {
        side = value;
    }

    const std::string &OrderCancelReplaceRequest::getTransactTime() const
    {
        return transactTime;
    }

    void OrderCancelReplaceRequest::setTransactTime(const std::string &value)
    {
        transactTime = value;
    }

    char OrderCancelReplaceRequest::getOrdType() const
    {
        return ordType;
    }

    void OrderCancelReplaceRequest::setOrdType(char value)
    {
        ordType = value;
    }

    // ORDER CANCEL REQUEST
    std::string OrderCancelRequest::serialize(std::string sendCompID, std::string targetCompID, int bodyLength)
    {
        setMsgType("9");
        setSenderCompID(sendCompID);
        setTargetCompID(targetCompID);
        setMsgSeqNum();
        setSendingTime();
        setBodyLength(bodyLength);

        std::ostringstream oss;

        oss << formatHeader();

        // Body
        oss << "37=" << getOrderID() << SOH
            << "11=" << getClOrdID() << SOH
            << "41=" << getOrigClOrdID() << SOH
            << "39=" << getOrdStatus() << SOH
            << "434=" << getCxlRejResponseTo() << SOH;

        oss << formatTrailer(oss.str());

        return oss.str();
    }

    // Getters and Setters
    const std::string &OrderCancelRequest::getOrderID() const
    {
        return orderID;
    }

    void OrderCancelRequest::setOrderID(const std::string &value)
    {
        orderID = value;
    }

    const std::string &OrderCancelRequest::getClOrdID() const
    {
        return clOrdID;
    }

    void OrderCancelRequest::setClOrdID(const std::string &value)
    {
        clOrdID = value;
    }

    const std::string &OrderCancelRequest::getOrigClOrdID() const
    {
        return origClOrdID;
    }

    void OrderCancelRequest::setOrigClOrdID(const std::string &value)
    {
        origClOrdID = value;
    }

    char OrderCancelRequest::getOrdStatus() const
    {
        return ordStatus;
    }

    void OrderCancelRequest::setOrdStatus(char value)
    {
        ordStatus = value;
    }

    int OrderCancelRequest::getCxlRejResponseTo() const
    {
        return cxlRejResponseTo;
    }

    void OrderCancelRequest::setCxlRejResponseTo(int value)
    {
        cxlRejResponseTo = value;
    }

    // EXECUTION REPORT
    std::string ExecutionReport::serialize(std::string sendCompID, std::string targetCompID, int bodyLength)
    {
        setMsgType("8");
        setSenderCompID(sendCompID);
        setTargetCompID(targetCompID);
        setMsgSeqNum();
        setSendingTime();
        setBodyLength(bodyLength);

        std::ostringstream oss;

        oss << formatHeader();

        // Body
        oss << "37=" << getOrderID() << SOH
            << "17=" << getExecID() << SOH
            << "150=" << getExecType() << SOH
            << "39=" << getOrdStatus() << SOH
            << "55=" << getSymbol() << SOH
            << "54=" << getSide() << SOH
            << "151=" << getLeavesQty() << SOH
            << "14=" << getCumQty() << SOH
            << "6=" << getAvgPx() << SOH;

        oss << formatTrailer(oss.str());

        return oss.str();
    }

    // Getters and Setters
    const std::string &ExecutionReport::getOrderID() const
    {
        return orderID;
    }

    void ExecutionReport::setOrderID(const std::string &value)
    {
        orderID = value;
    }

    const std::string &ExecutionReport::getExecID() const
    {
        return execID;
    }

    void ExecutionReport::setExecID(const std::string &value)
    {
        execID = value;
    }

    char ExecutionReport::getExecType() const
    {
        return execType;
    }

    void ExecutionReport::setExecType(char value)
    {
        execType = value;
    }

    char ExecutionReport::getOrdStatus() const
    {
        return ordStatus;
    }

    void ExecutionReport::setOrdStatus(char value)
    {
        ordStatus = value;
    }

    const std::string &ExecutionReport::getSymbol() const
    {
        return symbol;
    }

    void ExecutionReport::setSymbol(const std::string &value)
    {
        symbol = value;
    }

    char ExecutionReport::getSide() const
    {
        return side;
    }

    void ExecutionReport::setSide(char value)
    {
        side = value;
    }

    int ExecutionReport::getLeavesQty() const
    {
        return leavesQty;
    }

    void ExecutionReport::setLeavesQty(int value)
    {
        leavesQty = value;
    }

    int ExecutionReport::getCumQty() const
    {
        return cumQty;
    }

    void ExecutionReport::setCumQty(int value)
    {
        cumQty = value;
    }

    double ExecutionReport::getAvgPx() const
    {
        return avgPx;
    }

    void ExecutionReport::setAvgPx(double value)
    {
        avgPx = value;
    }

    // MARKET DATA SNAPSHOT FULL REFRESH
    std::string MarketDataSnapshotFullRefresh::serialize(std::string sendCompID, std::string targetCompID, int bodyLength)
    {
        setMsgType("W");
        setSenderCompID(sendCompID);
        setTargetCompID(targetCompID);
        setMsgSeqNum();
        setSendingTime();
        setBodyLength(bodyLength);

        std::ostringstream oss;

        oss << formatHeader();

        // Body
        oss << "268=" << getNoMDEntries() << SOH
            << "279=" << getMDUpdateAction() << SOH;

        oss << formatTrailer(oss.str());

        return oss.str();
    }

    // Getters and Setters
    int MarketDataSnapshotFullRefresh::getNoMDEntries() const
    {
        return noMDEntries;
    }

    void MarketDataSnapshotFullRefresh::setNoMDEntries(int value)
    {
        noMDEntries = value;
    }

    char MarketDataSnapshotFullRefresh::getMDUpdateAction() const
    {
        return mdUpdateAction;
    }

    void MarketDataSnapshotFullRefresh::setMDUpdateAction(char value)
    {
        mdUpdateAction = value;
    }

    // MARKET DATA INCREMENTAL
    std::string MarketDataIncrementalRefresh::serialize(std::string sendCompID, std::string targetCompID, int bodyLength)
    {
        setMsgType("X");
        setSenderCompID(sendCompID);
        setTargetCompID(targetCompID);
        setMsgSeqNum();
        setSendingTime();
        setBodyLength(bodyLength);

        std::ostringstream oss;

        oss << formatHeader();

        // Body
        oss << "55=" << getSymbol() << SOH
            << "268=" << getNoMDEntries() << SOH;

        oss << formatTrailer(oss.str());

        return oss.str();
    }

    // Getters and Setters
    const std::string &MarketDataIncrementalRefresh::getSymbol() const
    {
        return symbol;
    }

    void MarketDataIncrementalRefresh::setSymbol(const std::string &value)
    {
        symbol = value;
    }

    int MarketDataIncrementalRefresh::getNoMDEntries() const
    {
        return noMDEntries;
    }

    void MarketDataIncrementalRefresh::setNoMDEntries(int value)
    {
        noMDEntries = value;
    }

} // namespace FIXProtocol
