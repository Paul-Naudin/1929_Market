#pragma once

#include "../../Common/FIXProtocol/FIXProtocol.hpp"
#include "../../Market/OrderBook/OrderBook.h"
#include <string>

// Command interface
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

class HelpCommand : public Command {
public:
    HelpCommand() {}
    ~HelpCommand() {}

    void execute() override;
};

class LogonCommand : public Command {
public:
    LogonCommand(const std::string args);
    ~LogonCommand() {}

    void execute() override;
private:
    std::string username;
    std::string password;
};

class NewOrderCommand : public Command {
public:
    NewOrderCommand(const std::string args);
    ~NewOrderCommand() {}

    void execute() override;
private:
    std::string symbol;
    char side;
    int quantity;
    double price;
};

class CancelOrderCommand : public Command {
public:
    CancelOrderCommand(const std::string args);
    ~CancelOrderCommand() {}

    void execute() override;
private:
    std::string orderId;
};
