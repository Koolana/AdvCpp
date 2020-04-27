#include "connection.h"
#include <arpa/inet.h>
#include <utility>
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

Connection::Connection(Connection&& other) :
    _fd(other._fd.extract()),
    _flag_opened(false),
    _dst_addr(other._dst_addr),
    _dst_port(other._dst_port)
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

        for (size_t i = 0; i < temp_s; i++) { //записываем сколько сумели считали, чтобы в след раз попытаться прочесть всю пачку до конца
            _data.push_back(*((char*)data + i + s));
        }

        s += temp_s;
    }
}

void Connection::close()
{
    ::close(_fd.get_fd());
    _flag_opened = false;
}

void Connection::clear_data()
{
    _data.clear();
}

bool Connection::is_opened() const
{
    return _flag_opened;
}

bool Connection::is_data_ready() const
{
    return _data.size() >= _waitBytes;
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

void Connection::set_epoll_events(uint32_t events)
{
    _events = events;
}

void Connection::set_wait_bytes(size_t numBytes)
{
    _waitBytes = numBytes;
}

uint32_t Connection::get_epoll_events() const
{
    return _events;
}

const std::string Connection::getDstAddr() const
{
    return std::string(::inet_ntoa(_dst_addr));
}

uint16_t Connection::getDstPort() const
{
    return ::ntohs(_dst_port);
}

size_t Connection::get_count_bytes() const
{
    return _data.size();
}

std::string Connection::get_data() const
{
    return _data;
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
