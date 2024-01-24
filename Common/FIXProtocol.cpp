#include "FIXProtocol.hpp"
#include <vector>

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

    void FIXMessage::setMsgSeqNum(int value = 1)
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

    // Derived classes

    void LogonMessage::parseField(const std::string &field)
    {
        size_t equalPos = field.find('=');
        if (equalPos != std::string::npos)
        {
            std::string tag = field.substr(0, equalPos);
            std::string value = field.substr(equalPos + 1);

            switch (std::stoi(tag))
            {
            // Header
            case 9: // BodyLength
                setBodyLength(std::stoi(value));
                break;
            case 35: // MsgType
                setMsgType(value);
                break;
            case 49: // SenderCompID
                setSenderCompID(value);
                break;
            case 56: // TargetCompID
                setTargetCompID(value);
                break;
            case 34: // MsgSeqNum
                setMsgSeqNum(std::stoi(value));
                break;
            case 52: // SendingTime
                setSendingTime(value);
                break;

            // Body
            case 98: // EncryptMethod
                setEncryptMethod(value);
                break;
            case 108: // HeartBtInt
                setHeartBtInt(std::stoi(value));
                break;
            case 553: // Username
                setUsername(value);
                break;
            case 554: // Password
                setPassword(value);
                break;

            // Trailer
            case 10: // CheckSum
                setCheckSum(value);
                break;

            default:
                break;
            }
        }
    }

    const std::string LogonMessage::serialize(std::string sendCompID,
                                              std::string targetCompID, int bodyLength, int heartBtInt = 30,
                                              std::string user, std::string password)
    {
        setMsgType("A");
        setSenderCompID(sendCompID);
        setTargetCompID(targetCompID);
        setMsgSeqNum();
        setSendingTime();
        setBodyLength(bodyLength);

        std::ostringstream oss;

        // Header
        oss << "8=" << getBeginString() << SOH
            << "9=" << getBodyLength() << SOH
            << "35=" << getMsgType() << SOH
            << "49=" << getSenderCompID() << SOH
            << "56=" << getTargetCompID() << SOH
            << "34=" << getMsgSeqNum() << SOH
            << "52=" << getSendingTime() << SOH;

        // Body (Logon)
        setEncryptMethod("0");
        setHeartBtInt(heartBtInt);
        setUsername(user);
        setPassword(password);

        oss << "98=" << getEncryptMethod() << SOH
            << "108=" << getHeartBtInt() << SOH
            << "553=" << getUsername() << SOH
            << "554=" << getPassword() << SOH;

        // Trailer
        setCheckSum(oss.str());
        oss << "10=" << getCheckSum() << SOH;

        return oss.str();
    }

    void FIXProtocol::LogonMessage::deserialize(const std::string &data)
    {
        std::vector<std::string> fields;
        splitString(data, SOH, fields);

        for (const std::string &field : fields)
        {
            parseField(field);
        }
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

} // namespace FIXProtocol

void splitString(const std::string &input, char delimiter, std::vector<std::string> &result)
{
    std::istringstream stream(input);
    std::string token;
    while (std::getline(stream, token, delimiter))
    {
        result.push_back(token);
    }
}
