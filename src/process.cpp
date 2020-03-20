#include "process.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <signal.h>
#include <sys/wait.h>

Process::Process(const std::string& path)
{
    //Две пайпы, одна соединяет read родительского и stdout дочернего процесса
    //другая соединяет write родительского и stdin дочернего процесса
    if (pipe(pipefdIn) == -1) {
        perror("pipeIn");
        throw "Error open Pipe write";
    }

    if (pipe(pipefdOut) == -1) {
        perror("pipeOut");
        throw "Error open Pipe read";
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        throw "fork";
    }

    if (cpid == 0) { //Child
        ::close(pipefdIn[1]);
        ::close(pipefdOut[0]);

        char sTempPipeIn[32];
        char sTempPipeOut[32];

        sprintf(sTempPipeIn, "%d", pipefdIn[0]);
        sprintf(sTempPipeOut, "%d", pipefdOut[1]);

        //передаем концы пайп как аргументы в дочерний процесс
        if (execl(path.data(), path.data(), sTempPipeIn, sTempPipeOut) == -1) {
            throw "Error execl";
        }
    }
    else { //Parent
        ::close(pipefdIn[0]);
        ::close(pipefdOut[1]);
    }
}

Process::~Process()
{
    kill(cpid, SIGTERM); //посылаем сигнал на выключение дочернего процесса (?)
    waitpid(cpid, NULL, 0); //ждем завершения дочернего процесса (?)
}

//пишет определенное количество байт в поток ввода дочернего процесса,
//соединенного через пайп; вся проверка на пользователе
size_t Process::write(const void* data, size_t len)
{
    size_t numByte = ::write(pipefdIn[1], data, len);

    return numByte;
}

//пишет определенное количество байт в поток ввода дочернего процесса,
//соединенного через пайп; "зависнет", пока все не запишет
void Process::writeExact(const void* data, size_t len)
{
    size_t s = 0;
    size_t tempS = 0;

    while (s < len) {
        tempS = ::write(pipefdIn[1], data, len);

        if (tempS < 0) {
            throw "Error";
        }

        s += tempS;
    }
}

//читает определенное количество байт с потока вывода дочернего процесса,
//соединенного через пайп; вся проверка на пользователе
size_t Process::read(void* data, size_t len)
{
    size_t numByte = ::read(pipefdOut[0], data, len);

    return numByte;
}

//читает определенное количество байт с потока вывода дочернего процесса,
//соединенного через пайп; "зависнет", пока все их не считает
void Process::readExact(void* data, size_t len)
{
    size_t s = 0;
    size_t tempS = 0;
    char tempData[len];

    while (s < len) {
        tempS = ::read(pipefdOut[0], tempData, len - s);

        if (tempS < 0) {
            throw "Error";
        }

        for ( int i = 0; i < tempS; i++) {
            ((char*)data)[s + i] = tempData[i];
        }

        s += tempS;
    }
}

//проверка - открыт ли пайп для чтения
bool Process::isReadable() const
{
    return !::read(pipefdOut[0], NULL, 0);
}

//закрытие пайпа для записи
void Process::closeStdin()
{
    ::close(pipefdIn[1]);
}

//закрытие всех пайп
void Process::close()
{
    ::close(pipefdOut[0]);
    ::close(pipefdIn[1]);
}
