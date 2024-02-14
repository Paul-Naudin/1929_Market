#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/un.h>
#include <unistd.h>

#include <stdexcept>
#include <initializer_list>

#include <network.hpp>

namespace network
{

    network::network()
        : epollfd(0), running(0), eventmap(), error_flag(NETWORK_ERR_NONE)
    {
        epollfd = epoll_create1(EPOLL_CLOEXEC);

        if (epollfd < 0)
        {
            throw exception("epoll_create1(EPOLL_CLOEXEC) failed.", NETWORK_ERR_UNKNOWN);
        }

        ev_signal.fd = -1;
    }

    network::~network() noexcept
    {
        if (epollfd > 0)
            ::close(epollfd);
    }

    void network::clear_error_flag()
    {
        error_flag = NETWORK_ERR_NONE;
    }

    error_en network::get_error_flag()
    {
        return error_flag;
    }

    bool network::run()
    {

        struct sockaddr_storage peer;
        socklen_t plen;
        epoll_event events[NETWORK_DEFAULT_MAX_EVENTS];

        int nfds = 0;
        int timeout = NETWORK_DEFAULT_MAX_TIMEOUT;
        running = true;

        while (running != 0)
        {
            // Attente d'événements
            nfds = epoll_wait(epollfd, events, NETWORK_DEFAULT_MAX_EVENTS, timeout);

            // Gestion d'erreur
            if (nfds < 0)
            {
                if (errno == EINTR)
                    error_flag = NETWORK_ERR_HUP;
                else
                    error_flag = NETWORK_ERR_WAIT;
                running = false;
                return false;
            }
            else if (nfds == 0)
            {
                continue;
            }

            // Gestion des events
            for (int indx = 0; indx < nfds; indx++)
            {
                int fd = (int)events[indx].data.fd;
                mevent &ev = eventmap[fd];
                if (events[indx].events == 0)
                    continue;

                // Exécution du callback lié à l'event
                if (ev.callback && ev.fd > 0)
                {
                    if (ev.type == NETWORK_TYPE_ACC)
                    {
                        fd = accept4(ev.fd, (struct sockaddr *)&peer, &plen, SOCK_NONBLOCK);
                        if (fd > 0)
                        {
                            add_fio(ev.callback, fd, ev.evmask);
                        }
                    }
                    if (ev.callback(ev, events[indx].events) != NETWORK_ERR_NONE)
                    {
                        del(ev);
                    }
                }
            }
        }
        return true;
    }

    bool network::add(mevent ev)
    {
        clear_error_flag();

        if (ev.fd < 0 || eventmap.find(ev.fd) != eventmap.end())
        {
            error_flag = NETWORK_ERR_ADD;
            return false;
        }

        ev.event.data.fd = ev.fd;

        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, ev.fd, &ev.event) < 0)
        {
            error_flag = NETWORK_ERR_ADD;
            return false;
        }

        eventmap.insert(std::make_pair(ev.fd, ev));

        return true;
    }

    bool network::del(mevent &ev)
    {
        clear_error_flag();

        if (eventmap.find(ev.fd) == eventmap.end() || eventmap.erase(ev.fd) != 1)
        {
            error_flag = NETWORK_ERR_DEL;
        }
        else if (epoll_ctl(epollfd, EPOLL_CTL_DEL, ev.fd, &ev.event) < 0)
        {
            error_flag = NETWORK_ERR_DEL;
        }
        return (error_flag == NETWORK_ERR_NONE);
    }

    bool network::add_fio(callback_t callback, int fd, int evmask)
    {
        mevent ev;
        ev.type = NETWORK_TYPE_IO;
        ev.callback = callback;
        ev.event.events = evmask;
        ev.fd = fd;

        return add(ev);
    }

    bool network::add_timer(callback_t callback, int timeout, int period)
    {
        mevent ev;
        ev.type = NETWORK_TYPE_TIMER;
        ev.callback = callback;
        ev.event.events = NETWORK_ET | NETWORK_READ;
        ev.fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

        if (ev.fd < 0)
            throw exception("adding timer failed", NETWORK_ERR_TIMER);

        struct timespec ts;
        struct itimerspec itime;

        ts.tv_sec = timeout / 1000;
        ts.tv_nsec = (timeout % 1000) * 1000000;
        itime.it_value = ts;

        ts.tv_sec = period / 1000;
        ts.tv_nsec = (period % 1000) * 1000000;
        itime.it_interval = ts;

        if (timerfd_settime(ev.fd, 0, &itime, NULL) < 0)
        {
            ::close(ev.fd);
            ev.fd = -1;
            error_flag = NETWORK_ERR_TIMER;
            return false;
        }

        return add(ev);
    }

    bool network::add_signal(callback_t callback, int signum)
    {
        error_flag = NETWORK_ERR_SIGNAL;
        if (!callback)
            return false;

        if (ev_signal.fd < 0)
        {
            sigemptyset(&ev_signal.smask);

            ev_signal.type = NETWORK_TYPE_SIGNAL;
            ev_signal.event.events = NETWORK_READ;

            if (sigprocmask(SIG_BLOCK, &ev_signal.smask, NULL) == 0)
            {
                ev_signal.fd = signalfd(-1, &ev_signal.smask, SFD_NONBLOCK | SFD_CLOEXEC);
                if (ev_signal.fd <= 0)
                {
                    ev_signal.fd = -1;
                    return false;
                }
            }
            else
                return false;
        }

        if (eventmap.find(ev_signal.fd) != eventmap.end())
        {
            if (!del(ev_signal))
                return false;
        }

        sigaddset(&ev_signal.smask, signum);

        if (sigprocmask(SIG_BLOCK, &ev_signal.smask, NULL) == 0)
        {
            if (signalfd(ev_signal.fd, &ev_signal.smask, SFD_NONBLOCK | SFD_CLOEXEC) != ev_signal.fd)
            {
                return false;
            }
        }
        else
            return false;

        ev_signal.callback = callback;

        clear_error_flag();
        return add(ev_signal);
    }

    bool network::add_signal(callback_t callback, std::initializer_list<int> signums)
    {
        error_flag = NETWORK_ERR_SIGNAL;
        if (!callback)
            return false;

        for (auto elem : signums)
        {
            if (!add_signal(callback, elem))
                return false;
        }

        clear_error_flag();
        return true;
    }

    bool network::add_udp(callback_t callback, int stype, const char *straddr, int port, int evmask)
    {
        error_flag = NETWORK_ERR_UDP;
        mevent ev;
        ev.type = NETWORK_TYPE_IO;
        ev.callback = callback;
        ev.event.events = evmask;

        if (stype == NETWORK_IPV4 || stype == NETWORK_IPV6)
        {
            struct sockaddr_in baddr;
            memset(&baddr, 0x00, sizeof(struct sockaddr_in));
            baddr.sin_family = stype;
            baddr.sin_port = htons(port);
            if (inet_pton(stype, straddr, &baddr.sin_addr) != 1)
            {
                return false;
            }

            ev.fd = socket(stype, SOCK_DGRAM, 0);

            if (bind(ev.fd, (struct sockaddr *)&baddr, sizeof(struct sockaddr_in)) < 0)
            {
                if (ev.fd > 0)
                    ::close(ev.fd);
                return false;
            }
        }
        else if (stype == NETWORK_UNIX)
        {
            struct sockaddr_un baddr;
            baddr.sun_family = AF_UNIX;
            strncpy(baddr.sun_path, straddr, sizeof(baddr.sun_path) - 1);
            ev.fd = socket(AF_UNIX, SOCK_DGRAM, 0);

            if (bind(ev.fd, (struct sockaddr *)&baddr, sizeof(struct sockaddr_un)) < 0)
            {
                if (ev.fd > 0)
                    ::close(ev.fd);
                return false;
            }
        }
        else
        {
            return false;
        }

        clear_error_flag();
        return add(ev);
    }

    bool network::add_tcp(callback_t callback, int stype, const char *straddr, int port, int evmask)
    {
        error_flag = NETWORK_ERR_TCP;
        mevent ev;
        ev.type = NETWORK_TYPE_ACC;
        ev.callback = callback;
        ev.event.events = NETWORK_READ;
        ev.evmask = evmask;

        if (stype == NETWORK_IPV4 || stype == NETWORK_IPV6)
        {
            struct sockaddr_in baddr;
            memset(&baddr, 0x00, sizeof(struct sockaddr_in));
            baddr.sin_family = stype;
            baddr.sin_port = htons(port);
            if (inet_pton(stype, straddr, &baddr.sin_addr) != 1)
            {
                return false;
            }

            ev.fd = socket(stype, SOCK_STREAM, 0);

            if (bind(ev.fd, (struct sockaddr *)&baddr, sizeof(struct sockaddr_in)) < 0)
            {
                if (ev.fd > 0)
                    ::close(ev.fd);
                return false;
            }
        }
        else if (stype == NETWORK_UNIX)
        {
            struct sockaddr_un baddr;
            baddr.sun_family = AF_UNIX;
            strncpy(baddr.sun_path, straddr, sizeof(baddr.sun_path) - 1);
            ev.fd = socket(AF_UNIX, SOCK_STREAM, 0);

            if (bind(ev.fd, (struct sockaddr *)&baddr, sizeof(struct sockaddr_un)) < 0)
            {
                if (ev.fd > 0)
                    ::close(ev.fd);
                return false;
            }
        }
        else
        {
            return false;
        }

        int flags = 1;

#ifdef O_NONBLOCK
        flags = fcntl(ev.fd, F_GETFL, 0);
        if (flags < 0)
        {
            close(ev.fd);
            return false;
        }

        if (fcntl(ev.fd, F_SETFL, flags | O_NONBLOCK) < 0)
        {
            close(ev.fd);
            return false;
        }
#else
        if (ioctl(ev.fd, FIONBIO, &flags) < 0)
        {
            ::close(ev.fd);
            return NULL;
        }
#endif

        if (listen(ev.fd, NETWORK_DEFAULT_MAX_EVENTS) < 0)
        {
            ::close(ev.fd);
        }

        clear_error_flag();
        return add(ev);
    }

}