#pragma once

#include <string>

// Command interface
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

class HelpCommand : public Command {
public:
    HelpCommand() {}
    ~HelpCommand() {}

    void execute() override;
};

class LogonCommand : public Command {
public:
    LogonCommand(const std::string args);
    ~LogonCommand() {}

    void execute() override;
private:
    std::string username;
    std::string password;
};

class LogoutCommand : public Command {
public:
    LogoutCommand(const std::string args);
    ~LogoutCommand() {}

    void execute() override;
};