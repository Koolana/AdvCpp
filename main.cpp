#include <iostream>
#include "process.h"

int main()
{
    Process oneProc = Process("homeProc1");
    std::cout << "Number of written bytes - " << oneProc.write("test message\n", 13) << std::endl;
    oneProc.writeExact("123456", 6);

    char* tempStr = new char[255];

    if (oneProc.isReadable()) {
        std::cout << "Number of read bytes - " << oneProc.read(tempStr, 5) << std::endl; //считает сколько получится байт
        std::cout << "Read - " << tempStr << std::endl;

        oneProc.readExact((void*)tempStr, 14); //"зависнет" пока не считает все 14 байт
        std::cout << "ReadExact - " << tempStr << std::endl;
    }

    oneProc.closeStdin();
    oneProc.close();

    if (!oneProc.isReadable()) {
        std::cout << "pipe to read is closed" << std::endl;
    }

    return 0;
}

