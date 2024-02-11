#include "Command.h"
#include "FIXProtocol.hpp"

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
    } else {
        std::cout << "Logon command" << std::endl;
        std::cout << "Username: " << this->username << std::endl;
        std::cout << "Password: " << this->password << std::endl;
    }
    FIXProtocol::Logon logon("CLIENT", "42", 0, "0", 30, this->username, this->password);

    std::cout << logon.serialize() << std::endl;
}

// Logout command
LogoutCommand::LogoutCommand(const std::string args) {
}

void LogoutCommand::execute() {
}
