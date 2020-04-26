#ifndef CONNECTION
#define CONNECTION

#include <string>
#include <stdint.h>
#include <netinet/in.h>
#include "filedescriptor.h"

namespace tcp
{

class Connection
{
    friend class Server;
public:
    Connection(const std::string& addr, uint16_t port);
    ~Connection() {}

    size_t write(const void* data, size_t len);
    size_t read(void* data, size_t len);

    void writeExact(const void* data, size_t len);
    void readExact(void* data, size_t len);

    void close();
    bool is_opened() const;

    void set_timeout(int sec);

    const std::string getDstAddr() const;
    uint16_t getDstPort() const;

private:
    Connection(int fd, const sockaddr_in& addr);

    FileDescriptor _fd;

    in_addr _dst_addr; //в сетевом порядке
    uint16_t _dst_port; //в сетевом порядке

    bool _flag_opened;

    void connect(const std::string& addr, uint16_t port);
};

} //namespace tcp

#endif // CONNECTION
