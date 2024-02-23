#include "network.hpp"
#include "../FIXProtocol/FIXProtocol.hpp"

NetworkServer::NetworkServer()
{
    epoll_fd_ = -1;
    listening_fd_ = -1;
}

NetworkServer::~NetworkServer()
{
    if (epoll_fd_ != -1)
    {
        close(epoll_fd_);
    }
    if (listening_fd_ != -1)
    {
        close(listening_fd_);
    }
    for (const auto &client : clients_)
    {
        close(client.fd);
    }
}

bool NetworkServer::createEpoll()
{
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1)
    {
        std::cerr << "Failed to create epoll file descriptor" << std::endl;
        return false;
    }
    return true;
}

bool NetworkServer::setNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        std::cerr << "Failed to get file status flags" << std::endl;
        return false;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        std::cerr << "Failed to set non-blocking mode" << std::endl;
        return false;
    }
    return true;
}

void NetworkServer::addClient(int fd)
{
    ClientInfo newClient;
    newClient.fd = fd;

    clients_.push_back(newClient);

    std::cout << "Client " << fd << " added to the server." << std::endl;
}

bool NetworkServer::addSocket(int fd)
{
    epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        std::cerr << "Failed to add socket to epoll" << std::endl;
        return false;
    }
    return true;
}

void NetworkServer::removeSocket(int socket_fd)
{
    auto it = std::find_if(clients_.begin(), clients_.end(), [socket_fd](const ClientInfo &client)
                           { return client.fd == socket_fd; });

    if (it != clients_.end())
    {
        clients_.erase(it);
        std::cout << "Client with fd " << socket_fd << " removed" << std::endl;
    }
    else
    {
        std::cerr << "Client with fd " << socket_fd << " not found" << std::endl;
    }
}

bool NetworkServer::addListeningSocket(int port)
{
    listening_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_fd_ == -1)
    {
        std::cerr << "Failed to create listening socket" << std::endl;
        return false;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(listening_fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
    {
        std::cerr << "Failed to bind listening socket to port " << port << std::endl;
        return false;
    }

    if (listen(listening_fd_, SOMAXCONN) == -1)
    {
        std::cerr << "Failed to start listening on socket" << std::endl;
        return false;
    }

    if (!createEpoll() || !setNonBlocking(listening_fd_) || !addSocket(listening_fd_))
    {
        return false;
    }

    std::cout << "Listening for incoming connections on port " << port << std::endl;
    return true;
}

void NetworkServer::handleEvent(epoll_event event)
{
    // Handle client connection
    if (event.data.fd == listening_fd_)
    {
        sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(listening_fd_, reinterpret_cast<sockaddr *>(&client_addr), &addr_len);
        if (client_fd == -1)
        {
            std::cerr << "Failed to accept incoming connection" << std::endl;
        }
        else
        {
            setNonBlocking(client_fd);
            addSocket(client_fd);
            addClient(client_fd);

            std::cout << "New client connected with fd " << client_fd << std::endl;
        }
    }
    else
    {
        // Handle data from client
        int client_fd = event.data.fd;
        char buffer[1024];
        ssize_t bytesRead = read(client_fd, buffer, sizeof(buffer));
        if (bytesRead == -1)
        {
            // Error while reading data from client
            std::cerr << "Error reading data from client with fd " << client_fd << std::endl;
        }
        else if (bytesRead == 0)
        {
            // Client disconnected
            std::cout << "Client with fd " << client_fd << " closed the connection" << std::endl;
            close(client_fd);
            removeSocket(client_fd);
            for (const auto &client : clients_)
            {
                std::cout << client.fd << " ";
            }
            std::cout << std::endl;
        }
        else
        {
            // MUST ISOLATE THIS PART : GET MSG TYPE AND THEN EXPLOIT INFOS
            const std::string data = std::string(buffer, bytesRead);
            std::cout << "Received data from client with fd " << client_fd << ": " << data << std::endl;
            std::string messageType = MessageFactory::extractMsgType(data);
            std::unique_ptr<FIXMessage> message = MessageFactory::createMessage(messageType, data);

            if (message)
            {
                std::cout << "Message created" << std::endl;
                std::cout << message->getSenderCompID() << std::endl;
                Logon* test = dynamic_cast<Logon*>(message.get());
                std::cout <<  test->getUsername() << std::endl;
            }
        }
    }
}

void NetworkServer::run()
{
    if (epoll_fd_ == -1)
    {
        std::cerr << "Epoll file descriptor not initialized" << std::endl;
        return;
    }

    constexpr int MAX_EVENTS = 200;
    epoll_event events[MAX_EVENTS];

    while (true)
    {
        int num_events = epoll_wait(epoll_fd_, events, MAX_EVENTS, -1);
        if (num_events == -1)
        {
            std::cerr << "Error in epoll_wait" << std::endl;
            break;
        }
        for (int i = 0; i < num_events; ++i)
        {
            if (events[i].events & EPOLLHUP)
            {
                // Le client s'est déconnecté
                int client_fd = events[i].data.fd;
                std::cout << "Client " << client_fd << " disconnected." << std::endl;
                close(client_fd);
                removeSocket(client_fd);
                for (const auto &client : clients_)
                {
                    std::cout << client.fd << " ";
                }
                std::cout << std::endl;
            }
            else
            {
                handleEvent(events[i]);
            }
        }
    }
}
