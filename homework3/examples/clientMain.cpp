#include "connection.h"
#include "socketexception.h"
#include <iostream>

int main()
{
    try {
        tcp::Connection connection("127.0.0.1", 8888);
        connection.set_timeout(5);

        std::string str;
        std::cout << "Input: ";
        std::getline(std::cin, str);

        while(str != "stop") {
            size_t sizeStr = str.size();
            connection.writeExact(&sizeStr, sizeof(size_t));
            connection.writeExact(str.c_str(), sizeStr);

            size_t numChar;
            connection.readExact(&numChar, sizeof(size_t));
            std::cout << "Number of char from server: "<< numChar << std::endl;

            std::string msg;

            if(connection.is_opened() && numChar > 0) {
                msg.resize(numChar);
                connection.readExact(msg.begin().base(), numChar);

                std::cout << "From client: " << msg << std::endl;
            }

            std::cout << "Input: ";
            std::getline(std::cin, str);
        }

        connection.close();
    }
    catch(tcp::SocketException &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
