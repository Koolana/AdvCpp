#include "server.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "socketexception.h"

namespace tcp
{

Server::Server(const std::string& addr, uint16_t port, int numConnect, const Callback& handle) :
    _handler_callback(handle),
    _flag_opened(false),
    _sock_fd(::socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)),
    _epoll_fd(::epoll_create(1))
{
    if(_sock_fd.get_fd() < 0)
    {
        throw tcp::SocketException("Error created socket");
    }

    if (_epoll_fd.get_fd() < 0)
    {
        throw tcp::SocketException("Error creating epoll");;
    }

    open(addr, port, numConnect);
    add_epoll(_sock_fd.get_fd(), EPOLLIN);
}

void Server::start()
{
    const size_t EPOLL_SIZE = 128;
    epoll_event events[EPOLL_SIZE];

    while (is_opened())
    {
        int fd_count = epoll_wait(_epoll_fd.get_fd(), events, EPOLL_SIZE, -1);
        if (fd_count < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            throw tcp::SocketException("Error epoll_wait");
        }

        for (int i = 0; i < fd_count; ++i)
        {
            int fd = events[i].data.fd;

            if (fd == _sock_fd.get_fd())
            {
                accept_clients();
            }
            else
            {
                handle_client(fd, events[i].events);
            }
        }
    }
}

void Server::set_max_connect(int numConnect)
{
    if(::listen(_sock_fd.get_fd(), numConnect) < 0){
        throw tcp::SocketException("Error listen");
    }
}

void Server::open(const std::string& addr, uint16_t port, int numConnect)
{
    sockaddr_in dst_sock{};
    dst_sock.sin_family = AF_INET;
    dst_sock.sin_port = ::htons(port);

    if(::inet_aton(addr.c_str(), &dst_sock.sin_addr) < 0) {
        throw tcp::SocketException("Incorrect ip");
    }

    if(::bind(_sock_fd.get_fd(), reinterpret_cast<sockaddr*>(&dst_sock), sizeof(dst_sock)) < 0) {
        throw tcp::SocketException("Error bind");
    }

    if(::listen(_sock_fd.get_fd(), numConnect) < 0){
        throw tcp::SocketException("Error listen");
    }

    _flag_opened = true;
}

void Server::close()
{
    ::close(_sock_fd.get_fd());
    _flag_opened = false;
}

bool Server::is_opened() const
{
    return _flag_opened;
}

void Server::add_epoll(int fd, uint32_t events)
{
    epoll_event event{};
    event.data.fd = fd;
    event.events = events;

    if (epoll_ctl(_epoll_fd.get_fd(), EPOLL_CTL_ADD, fd, &event) < 0)
    {
        throw tcp::SocketException("Error add fd to epoll");
    }
}

void Server::accept_clients()
{
    while (true)
    {
        sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);

        int conn_fd = ::accept4(_sock_fd.get_fd(),
                                reinterpret_cast<sockaddr*>(&client_addr),
                                &addr_size,
                                SOCK_NONBLOCK);
        if (conn_fd < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return;
            }

            throw tcp::SocketException("Error accepting connection");
        }

        _connections.emplace(conn_fd, Connection(conn_fd, client_addr));

        add_epoll(conn_fd, EPOLLIN);
    }
}

void Server::handle_client(int fd, uint32_t events)
{
    Connection& connection = _connections.at(fd);
    connection.set_epoll_events(events);
    _handler_callback(connection);

    if (events & EPOLLHUP || events & EPOLLERR || events & EPOLLRDHUP)
    {
        _connections.erase(fd);
    }
}

} //namespace tcp
