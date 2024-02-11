#include "CommandFactory.h"

#include <string>

Command* CommandFactory::createCommand(const std::string &command, const std::string &args) {

    if (command == "help") {
        return new HelpCommand();
    } if (command == "Logon") {
        return new LogonCommand(args);
    } if (command == "Logout") {
        return new LogoutCommand(args);
    } else {
        return nullptr;
    }
    return nullptr;
}