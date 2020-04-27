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
                size_t numChar = 0;

                connection.readExact(&numChar, sizeof(size_t));
                std::cout << "Number of char from client: "<< numChar << std::endl;

                std::string msg;

                if(connection.is_opened() && numChar > 0) {
                    msg.resize(numChar);
                    connection.readExact(msg.begin().base(), numChar);

                    std::cout << "From client: " << msg << std::endl;

                    connection.writeExact(&numChar, sizeof(size_t));
                    connection.writeExact(msg.c_str(), numChar);
                }
            }

            std::cout << "Client disconnected" << std::endl;

            std::cout << "Wait next client (Y/N): ";
            std::cin >> c;

        } while (c != 'N' || c != 'n');
    }
    catch(tcp::SocketException &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
