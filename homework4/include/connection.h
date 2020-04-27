#ifndef CONNECTION
#define CONNECTION

#include <string>
#include <stdint.h>
#include <netinet/in.h>
#include "filedescriptor.h"

#include <iostream>

namespace tcp
{

class Connection
{
    friend class Server;
public:
    Connection(const std::string& addr, uint16_t port);
    Connection(int fd, const sockaddr_in& addr);
    Connection(Connection&& other);
    Connection(const Connection& other) = delete;
    ~Connection() {}

    size_t write(const void* data, size_t len);
    size_t read(void* data, size_t len);

    void writeExact(const void* data, size_t len);
    void readExact(void* data, size_t len);

    void close();
    void clear_data();
    bool is_opened() const;
    bool is_data_ready() const;

    void set_timeout(int sec);
    void set_epoll_events(uint32_t events);
    void set_wait_bytes(size_t numBytes);

    uint32_t get_epoll_events() const;
    const std::string getDstAddr() const;
    uint16_t getDstPort() const;
    size_t get_count_bytes() const;
    std::string get_data() const;

private:
    std::string _data;
    size_t _waitBytes;

    FileDescriptor _fd;
    uint32_t _events;

    in_addr _dst_addr; //в сетевом порядке
    uint16_t _dst_port; //в сетевом порядке

    bool _flag_opened;

    void connect(const std::string& addr, uint16_t port);
};

} //namespace tcp

#endif // CONNECTION
