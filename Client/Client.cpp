#include "Client.h"

#include <iostream>

void Client::executeCommand(const std::string &command, const std::string &args) {
    Command* cmd = commandFactory.createCommand(command, args);
    if (cmd != nullptr) {
        cmd->execute();
        delete cmd;
    } else {
        std::cout << "Invalid command" << std::endl;
    }
}