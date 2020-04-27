#ifndef SERVER
#define SERVER

#include "filedescriptor.h"
#include "connection.h"
#include <functional>
#include <map>

namespace tcp
{

class Server
{
    using Callback = std::function<void(Connection&)>;
public:
    Server(const std::string& addr, uint16_t port, int numConnect = 5, const Callback& handle = [](Connection&){});
    ~Server() {}

    void start();
    void set_max_connect(int num);

    void open(const std::string& addr, uint16_t port, int numConnect);
    void close();
    bool is_opened() const;

private:
    FileDescriptor _sock_fd;
    FileDescriptor _epoll_fd;
    Callback _handler_callback;

    std::map<int, Connection> _connections;

    bool _flag_opened;

    void add_epoll(int fd, uint32_t events);
    void accept_clients();
    void handle_client(int fd, uint32_t events);
};

} //namespace tcp

#endif // SERVER
