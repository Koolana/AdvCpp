#include <iostream>

#include "server.h"

int main()
{
    tcp::Server server("127.0.0.1", 8888);

    tcp::Connection connection = server.accept();
    connection.set_timeout(5);
    std::cout << "Connected: " << connection.getDstAddr() << ":" << connection.getDstPort() << std::endl;
    return 0;
}
