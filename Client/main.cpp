#include "GUI/Client.h"
#include <iostream>
#include <string>
#include <QApplication>
#include <QPushButton>
#include <QMainWindow>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QMainWindow window;

    window.show();

    return app.exec();

    Client client;
    std::string command;

    while (true)
    {
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        if (command == "exit" || command == "quit")
        {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        std::string commandName;
        std::string commandArgs;

        size_t spacePos = command.find(' ');
        if (spacePos != std::string::npos)
        {
            // If there is a space, separate the command and arguments
            commandName = command.substr(0, spacePos);
            commandArgs = command.substr(spacePos + 1);
        }
        else
        {
            // If there is no space, the whole string is the command
            commandName = command;
            commandArgs = "";
        }

        client.executeCommand(commandName, commandArgs);
    }

    return 0;
}
