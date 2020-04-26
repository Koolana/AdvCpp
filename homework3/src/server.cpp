#include "server.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socketexception.h"

namespace tcp
{

Server::Server(const std::string& addr, uint16_t port, int numConnect) :
    _flag_opened(false),
    _fd(::socket(PF_INET, SOCK_STREAM, 0))
{
    if(_fd.get_fd() < 0)
    {
        throw tcp::SocketException("Error created socket");
    }

    open(addr, port, numConnect);
}

Connection Server::accept()
{
    sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);

    int fd = ::accept(_fd.get_fd(), (sockaddr*)&client_addr, &addr_size);

    return Connection(fd, client_addr);
}

void Server::set_max_connect(int numConnect)
{
    if(::listen(_fd.get_fd(), numConnect) < 0){
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

    if(::bind(_fd.get_fd(), reinterpret_cast<sockaddr*>(&dst_sock), sizeof(dst_sock)) < 0) {
        throw tcp::SocketException("Error bind");
    }

    if(::listen(_fd.get_fd(), numConnect) < 0){
        throw tcp::SocketException("Error listen");
    }

    _flag_opened = true;
}

void Server::close()
{
    ::close(_fd.get_fd());
    _flag_opened = false;
}

bool Server::is_opened() const
{
    return _flag_opened;
}

} //namespace tcp
