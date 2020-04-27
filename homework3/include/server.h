#ifndef SERVER
#define SERVER

#include "filedescriptor.h"
#include "connection.h"

namespace tcp
{

class Server
{
public:
    Server(const std::string& addr, uint16_t port, int numConnect = 5);
    ~Server() {}

    Connection accept();

    void set_max_connect(int num);

    void open(const std::string& addr, uint16_t port, int numConnect);
    void close();
    bool is_opened() const;

private:
    FileDescriptor _fd;

    bool _flag_opened;
};

} //namespace tcp

#endif // SERVER
