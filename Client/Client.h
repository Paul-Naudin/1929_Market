#pragma once

#include "UI/CommandFactory.h"

class Client {
public:
    Client() {};
    ~Client() {};

    // Run the client
    void executeCommand(const std::string &command, const std::string &args);

private:
    CommandFactory commandFactory;
};