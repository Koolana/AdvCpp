#include "server.h"
#include "socketexception.h"
#include <iostream>

int main()
{
    try {
        tcp::Server server("127.0.0.1", 8888);

        char c;

        do{
            tcp::Connection connection = server.accept();
            connection.set_timeout(5);
            std::cout << "Connected: " << connection.getDstAddr() << ":" << connection.getDstPort() << std::endl;

            while(connection.is_opened()) {
                size_t numChar;
                connection.readExact(&numChar, sizeof(size_t));
                std::cout << "Number of char from client: "<< numChar << std::endl;

                char buff[numChar];
                connection.readExact(buff, numChar);
                std::cout << "From client: " << buff << std::endl;

                connection.writeExact(&numChar, sizeof(size_t));
                connection.writeExact(buff, numChar);
            }

            std::cout << "Client disconnected" << std::endl;

            std::cout << "Wait next client (Y/N)";
            std::cin >> c;

        } while (c != 'N');
    }
    catch(tcp::SocketException &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
