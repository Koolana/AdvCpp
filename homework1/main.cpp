#include <iostream>
#include <string>

#include "process.h"

int main(int argc, char *argv[])
{
    Process oneProc = Process("homeProc1");

    while (std::cin) {
        if (oneProc.isReadable()) {
            std::string tempStrIn;
            size_t numWrite = 0;
            size_t numRead = 0;
            char buff[256] = {}; //буфер для вывода, должен быть больше,
                                 //чем планируется считать

            std::cout << "Input str: ";
            std::getline(std::cin, tempStrIn);

            if ( tempStrIn == "exit") {
                oneProc.close();
                continue;
            }

            std::cout << "Number of char to write: ";
            std::cin >> numWrite;

            std::cout << "Number of char to read: ";
            std::cin >> numRead;

            //std::cout << "Number of written bytes - " << oneProc.write(tempStrIn, numWrite) << std::endl;
            oneProc.writeExact(tempStrIn.data(), numWrite); //гарантированно записать numWrite символов

            //std::cout << "Number of read bytes - " << oneProc.read(&buff, numRead) << std::endl;
            oneProc.readExact(&buff, numRead); //гарантированно считать numRead символов
            std::cout << "Output str: " << buff << std::endl << std::endl;
            std::cin.get();
        } else
        {
            std::cout << "Pipe isn't readable / closed" << std::endl;
            break;
        }
    }

    return 0;
}

