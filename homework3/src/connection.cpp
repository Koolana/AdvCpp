#include "connection.h"
#include <arpa/inet.h>
#include "socketexception.h"

namespace tcp
{

Connection::Connection(const std::string& addr, uint16_t port) :
    _flag_opened(false),
    _fd(::socket(PF_INET, SOCK_STREAM, 0))
{
    if(_fd.get_fd() < 0)
    {
        throw tcp::SocketException("Error created socket");
    }

    connect(addr, port);
}

Connection::Connection(int fd, const sockaddr_in& addr) :
    _flag_opened(true),
    _dst_addr(addr.sin_addr),
    _dst_port(addr.sin_port),
    _fd(fd)
{

}

size_t Connection::write(const void* data, size_t len)
{
    ssize_t numByte = ::write(_fd.get_fd(), data, len);

    if (numByte < 0) {
        throw tcp::SocketException("Error write");
    }

    return numByte;
}

size_t Connection::read(void* data, size_t len)
{
    ssize_t numByte = ::read(_fd.get_fd(), data, len);

    if (numByte < 0) {
        throw tcp::SocketException("Error read");
    }

    return numByte;
}

void Connection::writeExact(const void* data, size_t len)
{
    size_t s = 0;

    while (s < len) {
        s += write((char*)data + s, len - s);
    }
}

void Connection::readExact(void* data, size_t len)
{
    size_t s = 0;

    while (s < len) {
        size_t temp_s = read((char*)data + s, len - s);

        if(temp_s == 0) {
            _flag_opened = false;
            return;
        }

        s += temp_s;
    }
}

void Connection::close()
{
    ::close(_fd.get_fd());
    _flag_opened = false;
}

bool Connection::is_opened() const
{
    return _flag_opened;
}

void Connection::set_timeout(int sec)
{
    timeval timeout{sec, 0};

    if (::setsockopt(_fd.get_fd(),
                     SOL_SOCKET,
                     SO_SNDTIMEO,
                     &timeout,
                     sizeof(timeout)) < 0)
    {
        throw tcp::SocketException("Error setting timeout");
    }

    if (::setsockopt(_fd.get_fd(),
                     SOL_SOCKET,
                     SO_RCVTIMEO,
                     &timeout,
                     sizeof(timeout)) < 0)
    {
        throw tcp::SocketException("Error setting timeout");
    }
}

const std::string Connection::getDstAddr() const
{
    return std::string(::inet_ntoa(_dst_addr)); //быстрее чем return *(new std::string(::inet_ntoa(_dst_addr)));
                                                //т.к позволяет компилятору задействовать RVO (Return Value Optimization)
                                                //меньше системных вызовов
}

uint16_t Connection::getDstPort() const
{
    return ::ntohs(_dst_port);
}

void Connection::connect(const std::string& addr, uint16_t port)
{
    sockaddr_in dst_sock{};
    dst_sock.sin_family = AF_INET;
    dst_sock.sin_port = ::htons(port);

    if(::inet_aton(addr.c_str(), &dst_sock.sin_addr) < 0) {
        throw tcp::SocketException("Incorrect ip");
    }

    if (::connect(_fd.get_fd(), reinterpret_cast<sockaddr*>(&dst_sock), sizeof(dst_sock)) < 0) {
        throw tcp::SocketException("Error connect");
    }

    _dst_addr = dst_sock.sin_addr;
    _dst_port = dst_sock.sin_port;
    _flag_opened = true;
}

} //namespace tcp
