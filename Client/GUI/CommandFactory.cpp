#include "CommandFactory.h"

#include <string>

Command* CommandFactory::createCommand(const std::string &command, const std::string &args) {
    if (command == "help") {
        return new HelpCommand();
    }
    return nullptr;
}