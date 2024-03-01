#pragma once

#include "Command.h"
#include <string>

// Command factory
class CommandFactory
{
public:
    CommandFactory(/* args */) {};
    ~CommandFactory() {};

    // Create a command
    static Command* createCommand(const std::string &command, const std::string &args);
};
