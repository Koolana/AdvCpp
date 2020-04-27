#include <iostream>
#include <sys/epoll.h>

#include "server.h"
#include "socketexception.h"

#define NUM_BYTES_IN_PACK 5 //сколько байт ждем чтобы ответить, маленькое число для удобства проверки

int main()
{
    auto handler = [](tcp::Connection& connection)
    {
        if (connection.get_epoll_events() & EPOLLHUP || connection.get_epoll_events() & EPOLLERR || connection.get_epoll_events() & EPOLLRDHUP)
        {
            std::cout << "Disconnected: " << connection.getDstAddr() << ':' << connection.getDstPort() << std::endl;
            return;
        }

        if (connection.get_epoll_events() & EPOLLIN)
        {
            std::cout << "Connected: " << connection.getDstAddr() << ":" << connection.getDstPort() << std::endl;
            connection.set_wait_bytes(NUM_BYTES_IN_PACK);
            size_t leftBytes = NUM_BYTES_IN_PACK - connection.get_count_bytes();
            std::string msg;

            msg.resize(leftBytes);

            try {
                connection.readExact(msg.begin().base(), leftBytes);
            }
            catch(tcp::SocketException& tcpe){
                leftBytes = NUM_BYTES_IN_PACK - connection.get_count_bytes();
                connection.writeExact(&leftBytes, sizeof(size_t)); //если ошибка чтения, отправим сколько ещё осталось байт
            }

            if(connection.is_data_ready()){ //считали полностью всю пачку
                std::cout << "Package: " << connection.get_data() << std::endl;
                connection.writeExact(connection.get_data().c_str(), connection.get_count_bytes());
                connection.clear_data();
            }

            std::cout << "Left bytes: " << NUM_BYTES_IN_PACK - connection.get_count_bytes() << std::endl;
        }
    };

    try{
        tcp::Server server("127.0.0.1", 8888, 5, handler);

        server.start();
    }
    catch(tcp::SocketException& tcpe) {
        std::cout << tcpe.what() << std::endl;
    }

    return 0;
}
