// FIXprotocol.hpp
#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <locale>
#include <codecvt>
#include <iostream>
#include <memory>

const char SOH = '^'; // separator SOH
class FIXMessage
{
protected:
    // Header
    inline static const std::string beginString = "FIX.4.2"; // BeginString (8)
    int bodyLength;                                          // BodyLength (9)
    std::string msgType;                                     // MsgType (35)
    std::string senderCompID;                                // SenderCompID (49)
    std::string targetCompID;                                // TargetCompID (56)
    int msgSeqNum = 0;                                       // MsgSeqNum (34)
    std::string sendingTime;                                 // SendingTime (52)
    // Trailer
    int checkSum; // CheckSum (10)

public:
    FIXMessage(){};
    ~FIXMessage(){};

    FIXMessage deserialize(const std::string &message);

    // Getters and setters
    const std::string &getBeginString() const;

    int getBodyLength() const;
    void setBodyLength(int length);

    const std::string &getMsgType() const;
    void setMsgType(const std::string &value);

    const std::string &getSenderCompID() const;
    void setSenderCompID(const std::string &value);

    const std::string &getTargetCompID() const;
    void setTargetCompID(const std::string &value);

    int getMsgSeqNum() const;
    void setMsgSeqNum(int value = 1);

    const std::string &getSendingTime() const;
    void setSendingTime();
    void setSendingTime(std::string value);

    int getCheckSum() const;
    void setCheckSum(const std::string &message);

    std::string formatHeader() const;
    std::string formatTrailer(const std::string &message);
};

class Logon : public FIXMessage
{
private:
    std::string encryptMethod; // EncryptMethod (98)
    int heartBtInt;            // HeartBtInt (108)
    std::string username;      // Username (553)
    std::string password;      // Password (554)

public:
    Logon() : encryptMethod("0"), username(""), password("") {}
    Logon(std::string senderComp, std::string targetComp, int length, std::string encryptMethod = 0, int heartBtInt = 30, std::string username = "", std::string password = "");

    const std::string serialize();
    void deserialize(const std::string &message);

    // Getter and Setter
    const std::string &getEncryptMethod() const;
    void setEncryptMethod(const std::string &value);

    int getHeartBtInt() const;
    void setHeartBtInt(int value);

    const std::string &getUsername() const;
    void setUsername(const std::string &value);

    const std::string &getPassword() const;
    void setPassword(const std::string &value);
};

class NewOrder : public FIXMessage
{
private:
    std::string clOrdID;      // ClOrdID (11)
    char handlInst;           // HandlInst (21)
    std::string symbol;       // Symbol (55)
    char side;                // Side (54)
    std::string transactTime; // TransactTime (60)
    char ordType;             // OrdType (40)

public:
    NewOrder(){};
    NewOrder(std::string senderComp, std::string targetComp, int length, std::string clOrdID,
             char handlInst,
             std::string symbol,
             char side,
             std::string transactTime,
             char ordType);

    std::string serialize();
    void deserialize(const std::string &message);

    // Getters and setters
    const std::string &getClOrdID() const;
    void setClOrdID(const std::string &value);

    char getHandlInst() const;
    void setHandlInst(char value);

    const std::string &getSymbol() const;
    void setSymbol(const std::string &value);

    char getSide() const;
    void setSide(char value);

    const std::string &getTransactTime() const;
    void setTransactTime(const std::string &value);

    char getOrdType() const;
    void setOrdType(char value);
};

class OrderCancelReplaceRequest : public FIXMessage
{
private:
    std::string origClOrdID;  // OrigClOrdID (41)
    std::string clOrdID;      // ClOrdID (11)
    char handlInst;           // HandlInst (21)
    std::string symbol;       // Symbol (55)
    char side;                // Side (54)
    std::string transactTime; // TransactTime (60)
    char ordType;             // OrdType (40)

public:
    OrderCancelReplaceRequest(){};
    OrderCancelReplaceRequest(std::string senderComp, std::string targetComp, int length, std::string origClOrdID, std::string clOrdID,
                              char handlInst,
                              std::string symbol,
                              char side,
                              std::string transactTime,
                              char ordType);

    std::string serialize(std::string sendCompID, std::string targetCompID, int bodyLength);
    void deserialize(const std::string &message);

    // Getters and setters
    const std::string &getOrigClOrdID() const;
    void setOrigClOrdID(const std::string &value);

    const std::string &getClOrdID() const;
    void setClOrdID(const std::string &value);

    char getHandlInst() const;
    void setHandlInst(char value);

    const std::string &getSymbol() const;
    void setSymbol(const std::string &value);

    char getSide() const;
    void setSide(char value);

    const std::string &getTransactTime() const;
    void setTransactTime(const std::string &value);

    char getOrdType() const;
    void setOrdType(char value);
};

class OrderCancelRequest : public FIXMessage
{
private:
    std::string orderID;     // OrderID (37)
    std::string clOrdID;     // ClOrdID (11)
    std::string origClOrdID; // OrigClOrdID (41)
    char ordStatus;          // OrdStatus (39)
    int cxlRejResponseTo;    // CxlRejResponseTo (434)

public:
    OrderCancelRequest(){};
    OrderCancelRequest(std::string senderComp, std::string targetComp, int length, std::string orderID, std::string origClOrdID, std::string clOrdID,
                       char ordStatus,
                       int cxlRejResponseTo);

    std::string serialize(std::string sendCompID, std::string targetCompID, int bodyLength);
    void deserialize(const std::string &message);

    // Getters and setters
    const std::string &getOrderID() const;
    void setOrderID(const std::string &value);

    const std::string &getClOrdID() const;
    void setClOrdID(const std::string &value);

    const std::string &getOrigClOrdID() const;
    void setOrigClOrdID(const std::string &value);

    char getOrdStatus() const;
    void setOrdStatus(char value);

    int getCxlRejResponseTo() const;
    void setCxlRejResponseTo(int value);
};

class ExecutionReport : public FIXMessage
{
private:
    std::string orderID; // OrderID (37)
    std::string execID;  // ExecID (17)
    char execType;       // ExecType (150)
    char ordStatus;      // OrdStatus (39)
    std::string symbol;  // Symbol (55)
    char side;           // Side (54)
    int leavesQty;       // LeavesQty (151)
    int cumQty;          // CumQty (14)
    double avgPx;        // AvgPx (6)

public:
    ExecutionReport(){};
    ExecutionReport(std::string senderComp, std::string targetComp, int length,
                    std::string orderID, std::string execID, char execType,
                    char ordStatus, std::string symbol, char side,
                    int leavesQty, int cumQty, double avgPx);

    std::string serialize();
    void deserialize(const std::string &message);

    // Getters a,d setters
    const std::string &getOrderID() const;
    void setOrderID(const std::string &value);

    const std::string &getExecID() const;
    void setExecID(const std::string &value);

    char getExecType() const;
    void setExecType(char value);

    char getOrdStatus() const;
    void setOrdStatus(char value);

    const std::string &getSymbol() const;
    void setSymbol(const std::string &value);

    char getSide() const;
    void setSide(char value);

    int getLeavesQty() const;
    void setLeavesQty(int value);

    int getCumQty() const;
    void setCumQty(int value);

    double getAvgPx() const;
    void setAvgPx(double value);
};

class MarketDataSnapshotFullRefresh : public FIXMessage
{
private:
    int noMDEntries;     // NoMDEntries (268)
    char mdUpdateAction; // MDUpdateAction (279)

public:
    MarketDataSnapshotFullRefresh(){};
    MarketDataSnapshotFullRefresh(std::string senderComp, std::string targetComp, int length,
                                  int noMDEntries, char mdUpdateAction);

    std::string serialize();
    void deserialize(const std::string &message);

    // Getters and setters
    int getNoMDEntries() const;
    void setNoMDEntries(int value);

    char getMDUpdateAction() const;
    void setMDUpdateAction(char value);
};

class MarketDataIncrementalRefresh : public FIXMessage
{
private:
    std::string symbol; // Symbol (55)
    int noMDEntries;    // NoMDEntries (268)

public:
    MarketDataIncrementalRefresh(){};
    MarketDataIncrementalRefresh(std::string senderComp, std::string targetComp, int length,
                                 std::string symbol, int noMDEntries);

    std::string serialize();
    void deserialize(const std::string &message);

    // Getters and setters
    const std::string &getSymbol() const;
    void setSymbol(const std::string &value);

    int getNoMDEntries() const;
    void setNoMDEntries(int value);
};

class MessageFactory
{
public:
    static std::string extractMsgType(const std::string &message)
    {
        std::string type = "";
        size_t pos = message.find("35=");
        if (pos != std::string::npos)
        {

            pos += 3;
            size_t endPos = message.find_first_of('^', pos);
            if (endPos != std::string::npos)
            {
                type = message.substr(pos, endPos - pos);
            }
        }

        return type;
    }

    static std::unique_ptr<FIXMessage> createMessage(const std::string &message)
    {
        std::string msgType = extractMsgType(message);
        if (msgType == "A")
        {
            return std::make_unique<Logon>();
        }
        else if (msgType == "D")
        {
            return std::make_unique<NewOrder>();
        }
        else if (msgType == "G")
        {
            return std::make_unique<OrderCancelReplaceRequest>();
        }
        else if (msgType == "9")
        {
            return std::make_unique<OrderCancelRequest>();
        }
        else if (msgType == "8")
        {
            return std::make_unique<ExecutionReport>();
        }
        else if (msgType == "W")
        {
            return std::make_unique<MarketDataSnapshotFullRefresh>();
        }
        else if (msgType == "X")
        {
            return std::make_unique<MarketDataIncrementalRefresh>();
        }
        else
        {
            return nullptr; // Ou lancez une exception si nécessaire
        }
    }
};