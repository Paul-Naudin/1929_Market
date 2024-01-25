#include "GUI/Client.h"
#include <iostream>
#include <string>

int main() {
    Client client;
    std::string command;

    while (true) {
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        if (command == "exit") {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        // Separate command and arguments
        std::string commandName = command.substr(0, command.find(' '));
        std::string commandArgs = command.substr(command.find(' ') + 1);

        client.executeCommand(commandName, commandArgs);
    }
    
    return 0;
}
