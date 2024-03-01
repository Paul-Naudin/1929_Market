#include "Client.h"

int main() {
    Client client;
    std::string line;

    while (getline(std::cin, line)) {
        if (line == "exit" || line == "quit") {
            break;
        }
        std::string command = line.substr(0, line.find(" "));
        std::string args = line.find(" ") != std::string::npos ? line.substr(line.find(" ") + 1) : "";
        client.executeCommand(command, args);
    }
    
    return 0;
}
