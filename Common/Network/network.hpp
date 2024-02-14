#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>

#include <functional>
#include <unordered_map>
#include <exception>
#include <string>

#define NETWORK_DEFAULT_MAX_EVENTS    10
#define NETWORK_DEFAULT_MAX_TIMEOUT   2000

#define NETWORK_NONE          0
#define NETWORK_ERROR         EPOLLERR
#define NETWORK_READ          EPOLLIN
#define NETWORK_WRITE         EPOLLOUT
#define NETWORK_PRI           EPOLLPRI
#define NETWORK_HUP           EPOLLHUP
#define NETWORK_RDHUP         EPOLLRDHUP
#define NETWORK_ET            EPOLLET
#define NETWORK_ONESHOT       EPOLLONESHOT

#define NETWORK_IPV6          AF_INET6
#define NETWORK_IPV4          AF_INET
#define NETWORK_UNIX          AF_UNIX
namespace network
{

    enum type_en
    {
        NETWORK_TYPE_IO = 100,
        NETWORK_TYPE_SIGNAL = 101,
        NETWORK_TYPE_TIMER = 102,
        NETWORK_TYPE_ACC = 103,
    };

    enum error_en
    {
        NETWORK_ERR_NONE,
        NETWORK_ERR_UNKNOWN,
        NETWORK_ERR_NULL,
        NETWORK_ERR_ADD,
        NETWORK_ERR_DEL,
        NETWORK_ERR_HUP,
        NETWORK_ERR_WAIT,
        NETWORK_ERR_CLOSE,
        NETWORK_ERR_STOP,
        NETWORK_ERR_TIMER,
        NETWORK_ERR_SIGNAL,
        NETWORK_ERR_UDP,
        NETWORK_ERR_TCP,
        NETWORK_ERR_FIO
    };

    struct mevent;
    using callback_t = std::function<error_en(const mevent &, int)>;

    struct mevent
    {
        type_en type;
        epoll_event event;
        sigset_t smask;
        int evmask;
        int fd;
        callback_t callback;
    };

    class network
    {
    private:
        int epollfd;
        char running;
        std::unordered_map<int, mevent> eventmap;
        error_en error_flag;
        mevent ev_signal;

        bool add(mevent ev);
        bool del(mevent &ev);

    public:
        network();
        ~network() noexcept;

        bool add_timer(callback_t callback, int timeout, int period);
        bool add_fio(callback_t callback, int fd, int evmask);
        bool add_tcp(callback_t callback, int stype, const char *straddr, int port, int evmask);
        bool add_udp(callback_t callback, int stype, const char *straddr, int port, int evmask);
        bool add_signal(callback_t callback, int sig);
        bool add_signal(callback_t callback, std::initializer_list<int> signums);

        void clear_error_flag();
        error_en get_error_flag();

        bool run();
    };

    class exception : public std::exception
    {
    private:
        std::string message;
        error_en err;

    public:
        exception(const char *msg, error_en err = NETWORK_ERR_UNKNOWN)
            : message(msg), err(err)
        {
        }

        const char *what()
        {
            return message.c_str();
        }

        error_en get_error()
        {
            return err;
        }
    };

}
#endif