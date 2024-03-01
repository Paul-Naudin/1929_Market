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
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <queue>

#include "FIXProtocol.hpp"
#include "Exchange.h"
#include "OrderBook.h"
#include "Order.h"

struct ClientInfo
{
public:
    ClientInfo() : isLogged(false), mtx(std::make_unique<std::mutex>()), cv(std::make_unique<std::condition_variable>()) {}

    ClientInfo(const ClientInfo &other) : fd(other.fd), username(other.username), isLogged(other.isLogged), heartbeatInterval(other.heartbeatInterval), lastReceivedTime(other.lastReceivedTime), mtx(std::make_unique<std::mutex>()), cv(std::make_unique<std::condition_variable>()) {}

    ClientInfo(ClientInfo &&other) noexcept : fd(std::move(other.fd)), username(std::move(other.username)), isLogged(other.isLogged), heartbeatInterval(other.heartbeatInterval), lastReceivedTime(std::move(other.lastReceivedTime)), mtx(std::move(other.mtx)), cv(std::move(other.cv)) {}

    ClientInfo &operator=(ClientInfo &&other) noexcept
    {
        if (this != &other)
        {
            fd = std::move(other.fd);
            username = std::move(other.username);
            isLogged = other.isLogged;
            heartbeatInterval = other.heartbeatInterval;
            lastReceivedTime = std::move(other.lastReceivedTime);
            mtx = std::move(other.mtx);
            cv = std::move(other.cv);
        }
        return *this;
    }

    int fd;
    std::string username;
    bool isLogged;
    int heartbeatInterval;
    std::chrono::steady_clock::time_point lastReceivedTime;
    std::unique_ptr<std::mutex> mtx;
    std::unique_ptr<std::condition_variable> cv;
};

class ThreadPool
{
public:
    ThreadPool(size_t numThreads)
    {
        for (size_t i = 0; i < numThreads; ++i)
        {
            workers.emplace_back([this]
                                 {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return !tasks.empty() || stop; });
                        if (stop && tasks.empty()) {
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                } });
        }
    }

    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop)
            {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            tasks.emplace([task]()
                          { (*task)(); });
        }
        condition.notify_one();
        return result;
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers)
        {
            worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
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
    ThreadPool hbPool;
    Exchange bookOrdersManager;

    bool createEpoll();
    bool setNonBlocking(int fd);
    void addClient(int fd);
    bool addSocket(int fd);
    void removeSocket(int socket_fd);
    bool authenticateClient(const std::string &username, const std::string &password);
    void rejectConnection(const int client_fd, FIXMessage& message);
    void handleHeartbeat(ClientInfo &client, FIXMessage& message);
    void parseFix(const int client_fd, const std::string &rawData);
    void handleEvent(epoll_event event);
};

#endif /* NETWORK_SERVER_HPP */