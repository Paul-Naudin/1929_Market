#include "Command.h"

#include <iostream>

// Help command
void HelpCommand::execute() {
    std::cout << "Commands:" << std::endl;
    std::cout << "help" << std::endl;
    std::cout << "quit" << std::endl;
    std::cout << "add <price> <quantity> <bid/ask>" << std::endl;
    std::cout << "modify <id> <price> <quantity>" << std::endl;
    std::cout << "remove <id>" << std::endl;
    std::cout << "execute" << std::endl;
}