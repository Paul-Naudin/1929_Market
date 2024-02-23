#ifndef NETWORK_SERVER_HPP
#define NETWORK_SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include <algorithm>

struct ClientInfo
{
public:
    ClientInfo() : isLogged(false) {}

    int fd;
    std::string username;
    bool isLogged;
};
class NetworkServer
{
public:
    NetworkServer();
    ~NetworkServer();

    bool addListeningSocket(int port);
    void run();

private:
    int epoll_fd_;
    int listening_fd_;
    std::vector<ClientInfo> clients_;

    bool createEpoll();
    bool setNonBlocking(int fd);
    void addClient(int fd);
    bool addSocket(int fd);
    void removeSocket(int socket_fd);
    void parseFix(const std::string& rawData);
    void handleEvent(epoll_event event);
};

#endif /* NETWORK_SERVER_HPP */