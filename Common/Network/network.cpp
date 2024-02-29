#include "network.hpp"

NetworkServer::NetworkServer() : hbPool(4)
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

bool NetworkServer::authenticateClient(const std::string &username, const std::string &password)
{
    // const std::string credentialsFile = "credentials.txt";

    // std::ifstream file(credentialsFile);
    // if (!file.is_open())
    // {
    //     std::cerr << "Erreur : Impossible d'ouvrir le fichier des informations d'identification." << std::endl;
    //     return false;
    // }

    // std::unordered_map<std::string, std::string> credentials;
    // std::string line;
    // while (std::getline(file, line))
    // {
    //     std::istringstream iss(line);
    //     std::string user, pass;
    //     if (!(iss >> user >> pass))
    //     {
    //         std::cerr << "Erreur : Format de fichier invalide." << std::endl;
    //         return false;
    //     }
    //     credentials[user] = pass;
    // }

    // file.close();

    // auto it = credentials.find(username);
    // if (it != credentials.end() && it->second == password)
    // {
    //     return true;
    // }
    // else
    // {
    //     return false;
    // }
    return true;
}

void NetworkServer::rejectConnection(int client_fd, FIXMessage &message)
{
    FIXMessage logoutMessage;
    logoutMessage.setMsgType("5");
    logoutMessage.setSenderCompID(message.getSenderCompID());
    logoutMessage.setTargetCompID(message.getTargetCompID());
    logoutMessage.setSendingTime();

    std::string rejectionMessage = logoutMessage.serialize();
    send(client_fd, rejectionMessage.c_str(), rejectionMessage.length(), 0);
}

void NetworkServer::handleHeartbeat(ClientInfo &client, FIXMessage &message)
{
    while (client.isLogged)
    {
        std::unique_lock<std::mutex> lock(*client.mtx);
        if (client.cv->wait_for(lock, std::chrono::seconds(client.heartbeatInterval)) == std::cv_status::timeout)
        {
            std::cout << "Sending Test Request to client: " << client.fd << std::endl;
            message.setSendingTime();
            message.setBodyLength();
            send(client.fd, message.sendHeartbeat().c_str(), message.sendHeartbeat().length(), 0);
            client.lastReceivedTime = std::chrono::steady_clock::now();
        }
    }
}

void NetworkServer::parseFix(const int client_fd, const std::string &rawData)
{
    std::string messageType = MessageFactory::extractMsgType(rawData);
    std::unique_ptr<FIXMessage> message = MessageFactory::createMessage(messageType, rawData);

    if (message)
    {
        if (message->getMsgType() == "A")
        {
            if (Logon *logon = dynamic_cast<Logon *>(message.get()))
            {
                if (authenticateClient(logon->getUsername(), logon->getPassword()))
                {
                    for (auto &client : clients_)
                    {
                        if (client.fd == client_fd)
                        {
                            client.isLogged = true;
                            client.username = logon->getUsername();
                            client.heartbeatInterval = logon->getHeartBtInt();
                            hbPool.enqueue([this, &client, logon]()
                                           { handleHeartbeat(client, *logon); });
                            break;
                        }
                    }
                    std::string confirmationResponse = logon->serialize(true);
                    send(client_fd, confirmationResponse.c_str(), confirmationResponse.length(), 0);
                }
                else
                {
                    rejectConnection(client_fd, *logon);
                }
            }
        }
        else if (message->getMsgType() == "D")
        {
            if (NewOrder *newOrder = dynamic_cast<NewOrder *>(message.get()))
            {
                OrderBook &orderBook = bookOrdersManager.getOrderBook(newOrder->getSymbol());
                orderBook.addOrder(newOrder->getPrice(), newOrder->getOrderQty(), newOrder->getSide() == '1' ? OrderType::ASK : OrderType::BID);
                // bookOrdersManager.manageOrderInteraction(orderBook);
            }
        }
        else if (message->getMsgType() == "G")
        {
            if (auto orderCancelReplaceRequest = dynamic_cast<OrderCancelReplaceRequest *>(message.get()))
            {
                // CALL TO BOOKORDER AND UPDATES HERE
            }
        }
        else if (message->getMsgType() == "9")
        {
            if (auto orderCancelRequest = dynamic_cast<OrderCancelRequest *>(message.get()))
            {
                // CALL TO BOOKORDER AND UPDATES HERE
            }
        }
        else if (message->getMsgType() == "8")
        {
            if (auto executionReport = dynamic_cast<ExecutionReport *>(message.get()))
            {
                // CALL TO BOOKORDER AND UPDATES HERE
            }
        }
        else if (message->getMsgType() == "W")
        {
            if (auto marketDataSnapshot = dynamic_cast<MarketDataSnapshotFullRefresh *>(message.get()))
            {
                // CALL TO BOOKORDER AND UPDATES HERE
            }
        }
        else if (message->getMsgType() == "X")
        {
            if (auto marketDataIncremental = dynamic_cast<MarketDataIncrementalRefresh *>(message.get()))
            {
                // CALL TO BOOKORDER AND UPDATES HERE
            }
        }
        else
        {
            std::cout << "Unknown message type." << std::endl;
            throw std::runtime_error("Unknown message type encountered.");
        }
    }
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
            const std::string data = std::string(buffer, bytesRead);
            std::cout << "Received data from client with fd " << client_fd << ": " << data << std::endl;
            parseFix(client_fd, data);
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
