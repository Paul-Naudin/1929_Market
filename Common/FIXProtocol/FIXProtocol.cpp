#include "FIXProtocol.hpp"
#include <vector>

void FIXMessage::parseRawData(const std::string &rawData)
{
    size_t pos = 0;

    pos = rawData.find("9=");
    if (pos != std::string::npos)
    {
        pos += 2;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setBodyLength(std::stoi(rawData.substr(pos, endPos - pos)));
        }
    }

    pos = rawData.find("35=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setMsgType(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("49=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setSenderCompID(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("56=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setTargetCompID(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("34=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setMsgSeqNum(std::stoi(rawData.substr(pos, endPos - pos)));
        }
    }

    pos = rawData.find("52=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setSendingTime(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("10=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setCheckSum(rawData.substr(pos, endPos - pos));
        }
    }
}

FIXMessage FIXMessage::deserialize(const std::string &message)
{
}

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
Logon::Logon(const std::string &rawData) : FIXMessage(rawData)
{
    parseRawData(rawData);
}

void Logon::parseRawData(const std::string &rawData)
{
    size_t pos = 0;

    pos = rawData.find("98=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setEncryptMethod(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("108=");
    if (pos != std::string::npos)
    {
        pos += 4;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setHeartBtInt(std::stoi(rawData.substr(pos, endPos - pos)));
        }
    }

    pos = rawData.find("553=");
    if (pos != std::string::npos)
    {
        pos += 4;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setUsername(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("554=");
    if (pos != std::string::npos)
    {
        pos += 4;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setPassword(rawData.substr(pos, endPos - pos));
        }
    }
}

const std::string Logon::serialize()
{
    std::ostringstream oss;

    // Header
    oss << formatHeader();

    // Body
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

void Logon::deserialize(const std::string &message)
{
    std::istringstream iss(message);
    std::string field;

    while (std::getline(iss, field, '^'))
    {
        size_t pos = field.find('=');
        if (pos != std::string::npos)
        {
            std::string tag = field.substr(0, pos);
            std::string value = field.substr(pos + 1);

            switch (std::stoi(tag))
            {
            case 49:
                setSenderCompID(value);
                break;
            case 56:
                setTargetCompID(value);
                break;
            case 34:
                setMsgSeqNum(std::stoi(value));
                break;
            case 52:
                setSendingTime(value);
                break;
            case 98:
                setEncryptMethod(value);
                break;
            case 108:
                setHeartBtInt(std::stoi(value));
                break;
            default:
                break;
            }
        }
    }
}

const std::string &Logon::getEncryptMethod() const
{
    return encryptMethod;
}

void Logon::setEncryptMethod(const std::string &value)
{
    encryptMethod = value;
}

int Logon::getHeartBtInt() const
{
    return heartBtInt;
}

void Logon::setHeartBtInt(int value)
{
    heartBtInt = value;
}

const std::string &Logon::getUsername() const
{
    return username;
}

void Logon::setUsername(const std::string &value)
{
    username = value;
}

const std::string &Logon::getPassword() const
{
    return password;
}

void Logon::setPassword(const std::string &value)
{
    password = value;
}

// NEW ORDER
NewOrder::NewOrder(const std::string &rawData) : FIXMessage(rawData)
{
    size_t pos = rawData.find("11=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setClOrdID(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("21=");
    if (pos != std::string::npos)
    {
        pos += 3;
        setHandlInst(rawData[pos]);
    }

    pos = rawData.find("55=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setSymbol(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("54=");
    if (pos != std::string::npos)
    {
        pos += 3;
        setSide(rawData[pos]);
    }

    pos = rawData.find("60=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setTransactTime(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("40=");
    if (pos != std::string::npos)
    {
        pos += 3;
        setOrdType(rawData[pos]);
    }
}

std::string NewOrder::serialize()
{
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

void NewOrder::deserialize(const std::string &message)
{
    std::istringstream iss(message);
    std::string field;

    while (std::getline(iss, field, '^'))
    {
        size_t pos = field.find('=');
        if (pos != std::string::npos)
        {
            std::string tag = field.substr(0, pos);
            std::string value = field.substr(pos + 1);

            switch (std::stoi(tag))
            {
            case 11:
                setClOrdID(value);
                break;
            case 21:
                setHandlInst(value[0]);
                break;
            case 55:
                setSymbol(value);
                break;
            case 54:
                setSide(value[0]);
                break;
            case 60:
                setTransactTime(value);
                break;
            case 40:
                setOrdType(value[0]);
                break;
            default:
                break;
            }
        }
    }
}

// Getters and setters
const std::string &NewOrder::getClOrdID() const
{
    return clOrdID;
}

void NewOrder::setClOrdID(const std::string &value)
{
    clOrdID = value;
}

char NewOrder::getHandlInst() const
{
    return handlInst;
}

void NewOrder::setHandlInst(char value)
{
    handlInst = value;
}

const std::string &NewOrder::getSymbol() const
{
    return symbol;
}

void NewOrder::setSymbol(const std::string &value)
{
    symbol = value;
}

char NewOrder::getSide() const
{
    return side;
}

void NewOrder::setSide(char value)
{
    side = value;
}

const std::string &NewOrder::getTransactTime() const
{
    return transactTime;
}

void NewOrder::setTransactTime(const std::string &value)
{
    transactTime = value;
}

char NewOrder::getOrdType() const
{
    return ordType;
}

void NewOrder::setOrdType(char value)
{
    ordType = value;
}

// ORDER CANCEL / REPLACE REQUEST

OrderCancelReplaceRequest::OrderCancelReplaceRequest(const std::string &rawData) : FIXMessage(rawData)
{
    size_t pos = rawData.find("41=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setOrigClOrdID(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("11=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setClOrdID(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("21=");
    if (pos != std::string::npos)
    {
        pos += 3;
        setHandlInst(rawData[pos]);
    }

    pos = rawData.find("55=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setSymbol(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("54=");
    if (pos != std::string::npos)
    {
        pos += 3;
        setSide(rawData[pos]);
    }

    pos = rawData.find("60=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setTransactTime(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("40=");
    if (pos != std::string::npos)
    {
        pos += 3;
        setOrdType(rawData[pos]);
    }
}

std::string OrderCancelReplaceRequest::serialize(std::string sendCompID, std::string targetCompID, int bodyLength)
{
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

void OrderCancelReplaceRequest::deserialize(const std::string &message)
{
    std::istringstream iss(message);
    std::string field;

    while (std::getline(iss, field, '^'))
    {
        size_t pos = field.find('=');
        if (pos != std::string::npos)
        {
            std::string tag = field.substr(0, pos);
            std::string value = field.substr(pos + 1);

            switch (std::stoi(tag))
            {
            case 41:
                setOrigClOrdID(value);
                break;
            case 11:
                setClOrdID(value);
                break;
            case 21:
                setHandlInst(value[0]);
                break;
            case 55:
                setSymbol(value);
                break;
            case 54:
                setSide(value[0]);
                break;
            case 60:
                setTransactTime(value);
                break;
            case 40:
                setOrdType(value[0]);
                break;
            default:
                break;
            }
        }
    }
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
OrderCancelRequest::OrderCancelRequest(const std::string &rawData) : FIXMessage(rawData)
{
    size_t pos = rawData.find("37=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setOrderID(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("11=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setClOrdID(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("41=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setOrigClOrdID(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("39=");
    if (pos != std::string::npos)
    {
        pos += 3;
        setOrdStatus(rawData[pos]);
    }

    pos = rawData.find("434=");
    if (pos != std::string::npos)
    {
        pos += 4;
        setCxlRejResponseTo(std::stoi(rawData.substr(pos, rawData.find('^', pos) - pos)));
    }
}

std::string OrderCancelRequest::serialize(std::string sendCompID, std::string targetCompID, int bodyLength)
{
    std::ostringstream oss;

    oss << formatHeader();

    // Body
    oss << "37=" << orderID << SOH
        << "11=" << clOrdID << SOH
        << "41=" << origClOrdID << SOH
        << "39=" << ordStatus << SOH
        << "434=" << cxlRejResponseTo << SOH;

    oss << formatTrailer(oss.str());

    return oss.str();
}

void OrderCancelRequest::deserialize(const std::string &message)
{
    std::istringstream iss(message);
    std::string field;

    while (std::getline(iss, field, '^'))
    {
        size_t pos = field.find('=');
        if (pos != std::string::npos)
        {
            std::string tag = field.substr(0, pos);
            std::string value = field.substr(pos + 1);

            switch (std::stoi(tag))
            {
            case 11:
                setClOrdID(value);
                break;
            case 37:
                setOrigClOrdID(value);
                break;
            case 41:
                setOrigClOrdID(value);
                break;
            case 39:
                setOrdStatus(value[0]);
                break;
            case 434:
                setCxlRejResponseTo(std::stoi(value));
                break;
            default:
                break;
            }
        }
    }
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
ExecutionReport::ExecutionReport(const std::string &rawData) : FIXMessage(rawData)
{
    size_t pos = rawData.find("37=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setOrderID(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("17=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setExecID(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("150=");
    if (pos != std::string::npos)
    {
        pos += 4;
        setExecType(rawData[pos]);
    }

    pos = rawData.find("39=");
    if (pos != std::string::npos)
    {
        pos += 3;
        setOrdStatus(rawData[pos]);
    }

    pos = rawData.find("55=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setSymbol(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("54=");
    if (pos != std::string::npos)
    {
        pos += 3;
        setSide(rawData[pos]);
    }

    pos = rawData.find("151=");
    if (pos != std::string::npos)
    {
        pos += 4;
        setLeavesQty(std::stoi(rawData.substr(pos, rawData.find('^', pos) - pos)));
    }

    pos = rawData.find("14=");
    if (pos != std::string::npos)
    {
        pos += 3;
        setCumQty(std::stoi(rawData.substr(pos, rawData.find('^', pos) - pos)));
    }

    pos = rawData.find("6=");
    if (pos != std::string::npos)
    {
        pos += 2;
        setAvgPx(std::stod(rawData.substr(pos, rawData.find('^', pos) - pos)));
    }
}

std::string ExecutionReport::serialize()
{
    std::ostringstream oss;

    oss << formatHeader();

    // Body
    oss << "37=" << orderID << SOH
        << "17=" << execID << SOH
        << "150=" << execType << SOH
        << "39=" << ordStatus << SOH
        << "55=" << symbol << SOH
        << "54=" << side << SOH
        << "151=" << leavesQty << SOH
        << "14=" << cumQty << SOH
        << "6=" << avgPx << SOH;

    oss << formatTrailer(oss.str());

    return oss.str();
}

void ExecutionReport::deserialize(const std::string &message)
{
    std::istringstream iss(message);
    std::string field;

    while (std::getline(iss, field, '^'))
    {
        size_t pos = field.find('=');
        if (pos != std::string::npos)
        {
            std::string tag = field.substr(0, pos);
            std::string value = field.substr(pos + 1);

            switch (std::stoi(tag))
            {
            case 37:
                setOrderID(value);
                break;
            case 17:
                setExecID(value);
                break;
            case 150:
                setExecType(value[0]);
                break;
            case 39:
                setOrdStatus(value[0]);
                break;
            case 55:
                setSymbol(value);
                break;
            case 54:
                setSide(value[0]);
                break;
            case 151:
                setLeavesQty(std::stoi(value));
                break;
            case 14:
                setCumQty(std::stoi(value));
                break;
            case 6:
                setAvgPx(std::stod(value));
                break;
            default:
                break;
            }
        }
    }
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

MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh(const std::string &rawData) : FIXMessage(rawData)
{
    size_t pos = rawData.find("268=");
    if (pos != std::string::npos)
    {
        pos += 4;
        setNoMDEntries(std::stoi(rawData.substr(pos, rawData.find('^', pos) - pos)));
    }

    pos = rawData.find("279=");
    if (pos != std::string::npos)
    {
        pos += 4;
        setMDUpdateAction(rawData[pos]);
    }
}

std::string MarketDataSnapshotFullRefresh::serialize()
{
    std::ostringstream oss;

    oss << formatHeader();

    // Body
    oss << "268=" << noMDEntries << SOH
        << "279=" << mdUpdateAction << SOH;

    oss << formatTrailer(oss.str());

    return oss.str();
}

void MarketDataSnapshotFullRefresh::deserialize(const std::string &message)
{
    std::istringstream iss(message);
    std::string field;

    while (std::getline(iss, field, '^'))
    {
        size_t pos = field.find('=');
        if (pos != std::string::npos)
        {
            std::string tag = field.substr(0, pos);
            std::string value = field.substr(pos + 1);

            switch (std::stoi(tag))
            {
            case 268:
                setNoMDEntries(std::stoi(value));
                break;
            case 279:
                setMDUpdateAction(value[0]);
                break;
            default:
                break;
            }
        }
    }
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

MarketDataIncrementalRefresh::MarketDataIncrementalRefresh(const std::string &rawData) : FIXMessage(rawData)
{
    size_t pos = rawData.find("55=");
    if (pos != std::string::npos)
    {
        pos += 3;
        size_t endPos = rawData.find('^', pos);
        if (endPos != std::string::npos)
        {
            setSymbol(rawData.substr(pos, endPos - pos));
        }
    }

    pos = rawData.find("268=");
    if (pos != std::string::npos)
    {
        pos += 4;
        setNoMDEntries(std::stoi(rawData.substr(pos, rawData.find('^', pos) - pos)));
    }
}

std::string MarketDataIncrementalRefresh::serialize()
{
    std::ostringstream oss;

    oss << formatHeader();

    // Body
    oss << "55=" << symbol << SOH
        << "268=" << noMDEntries << SOH;

    oss << formatTrailer(oss.str());

    return oss.str();
}

void MarketDataIncrementalRefresh::deserialize(const std::string &message)
{
    std::istringstream iss(message);
    std::string field;

    while (std::getline(iss, field, '^'))
    {
        size_t pos = field.find('=');
        if (pos != std::string::npos)
        {
            std::string tag = field.substr(0, pos);
            std::string value = field.substr(pos + 1);

            switch (std::stoi(tag))
            {
            case 55:
                setSymbol(value);
                break;
            case 268:
                setNoMDEntries(std::stoi(value));
                break;
            default:
                break;
            }
        }
    }
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

// MESSAGE FACTORY

MessageFactory::MessageFactoryMap MessageFactory::messageFactoryMap{
    {"A", [](const std::string &rawData) -> std::unique_ptr<FIXMessage>
     { return std::make_unique<Logon>(rawData); }},
    {"D", [](const std::string &rawData) -> std::unique_ptr<FIXMessage>
     { return std::make_unique<NewOrder>(rawData); }},
    {"G", [](const std::string &rawData) -> std::unique_ptr<FIXMessage>
     { return std::make_unique<OrderCancelReplaceRequest>(rawData); }},
    {"9", [](const std::string &rawData) -> std::unique_ptr<FIXMessage>
     { return std::make_unique<OrderCancelRequest>(rawData); }},
    {"8", [](const std::string &rawData) -> std::unique_ptr<FIXMessage>
     { return std::make_unique<ExecutionReport>(rawData); }},
    {"W", [](const std::string &rawData) -> std::unique_ptr<FIXMessage>
     { return std::make_unique<MarketDataSnapshotFullRefresh>(rawData); }},
    {"X", [](const std::string &rawData) -> std::unique_ptr<FIXMessage>
     { return std::make_unique<MarketDataIncrementalRefresh>(rawData); }},
};

std::string MessageFactory::extractMsgType(const std::string &message)
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

std::unique_ptr<FIXMessage> MessageFactory::createMessage(const std::string &messageType, const std::string &data)
{
    auto it = messageFactoryMap.find(messageType);
    if (it != messageFactoryMap.end())
    {
        return (it->second)(data);
    }
    else
    {
        return nullptr;
    }
}