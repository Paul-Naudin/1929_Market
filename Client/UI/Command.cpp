#include "Command.h"

#include <iostream>

// Help command
void HelpCommand::execute() {
    std::cout << "Usage: <command> [arguments]" << std::endl << std::endl;
    std::cout << "Available commands:" << std::endl << std::endl;
    std::cout << "\t" << "help" << "\t\t" << "Displays this help message" << std::endl;
    std::cout << "\t" << "exit," << "\t" << "quit" << "\t" << "Exits the program" << std::endl;
    std::cout << "\t" << "Logon" << "\t\t" << "Establish connection to remote market." << std::endl;
    std::cout << "\t" << "Logout" << "\t\t" << "Close connection to remote market." << std::endl;
}

// Logon command
LogonCommand::LogonCommand(const std::string args) {
    username = args.substr(0, args.find(" "));
    password = args.substr(args.find(" ") + 1);
}

void LogonCommand::execute() {
    if (this->username.length() <= 0 || this->password.length() <= 0) {
        std::cout << "Usage: Logon <username> <password>" << std::endl;
        return;
    }
    Logon logon("");

    logon.setSenderCompID("CLIENT");
    logon.setTargetCompID("MARKET");
    logon.setMsgSeqNum(1);
    logon.setSendingTime();
    logon.setMsgType("A");
    logon.setHeartBtInt(30);
    logon.setEncryptMethod("0");
    logon.setUsername(this->username);
    logon.setPassword(this->password);

    std::string message = logon.serialize(false);
    std::cout << "Sending Logon message: " << message << std::endl;
}

NewOrderCommand::NewOrderCommand(const std::string args) {
    std::string subargs = args;

    if (subargs.find(" ") == std::string::npos) {
        return;
    }
    symbol = subargs.substr(0, subargs.find(" "));
    subargs = subargs.substr(subargs.find(" ") + 1);
    side = subargs[0];
    subargs = subargs.substr(subargs.find(" ") + 1);
    quantity = std::stoi(subargs.substr(0, subargs.find(" ")));
    subargs = subargs.substr(subargs.find(" ") + 1);
    price = std::stod(subargs);
}

void NewOrderCommand::execute() {
    if (this->symbol.length() <= 0 || this->side < '0' || this->side > '1' || this->quantity <= 0 || this->price <= 0) {
        std::cout << "Usage: NewOrder <symbol> <side> <quantity> <price>" << std::endl;
        return;
    }
    NewOrder newOrder("");

    newOrder.setSenderCompID("CLIENT");
    newOrder.setTargetCompID("MARKET");
    newOrder.setMsgSeqNum(1);
    newOrder.setSendingTime();
    newOrder.setMsgType("D");
    newOrder.setSymbol(this->symbol);
    newOrder.setSide(this->side);
    newOrder.setOrderQty(this->quantity);
    newOrder.setPrice(this->price);

    std::string message = newOrder.serialize();
    std::cout << "Sending NewOrder message: " << message << std::endl;
}