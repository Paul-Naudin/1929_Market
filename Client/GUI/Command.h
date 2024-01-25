#pragma once

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