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
    int pipefdIn[2];
    int pipefdOut[2];
    isReadableFlag = false;

    if (pipe(pipefdIn) == -1) {
        throw std::exception();
    }

    if (pipe(pipefdOut) == -1) {
        throw std::exception();
    }

    cpid = fork();
    if (cpid == -1) {
        throw std::exception();
    }

    if (cpid == 0) { //Child
        ::close(pipefdIn[1]);
        ::close(pipefdOut[0]);

        dup2(pipefdIn[0], STDIN_FILENO);
        dup2(pipefdOut[1], STDOUT_FILENO);

        if (execl(path.data(), path.data()) == -1) {
            throw std::exception();
        }
    }
    else { //Parent
        ::close(pipefdIn[0]);
        ::close(pipefdOut[1]);

        pipefd_in = pipefdIn[1];
        pipefd_out = pipefdOut[0];

        isReadableFlag = true;
    }
}

Process::~Process()
{
    closeStdin();
    closeStdout();
    close();
}

//пишет определенное количество байт в поток ввода дочернего процесса,
//соединенного через пайп;
size_t Process::write(const void* data, size_t len)
{
    ssize_t numByte = ::write(pipefd_in, data, len);

    if (numByte < 0) { //если произошла ошибка записи
        throw std::exception();
    }

    return numByte; //неявное преобразование, здесь numByte >= 0 всегда
}

//пишет определенное количество байт в поток ввода дочернего процесса,
//соединенного через пайп; "зависнет", пока все не запишет
void Process::writeExact(const void* data, size_t len)
{
    size_t s = 0;
    ssize_t tempS = 0;

    while (s < len) {
        tempS = ::write(pipefd_in, data, len);

        if (tempS < 0) { //если произошла ошибка записи
            throw std::exception();
        }

        s += tempS; //неявное преобразование, здесь numByte >= 0 всегда
    }
}

//читает определенное количество байт с потока вывода дочернего процесса,
//соединенного через пайп;
size_t Process::read(void* data, size_t len)
{
    ssize_t numByte = ::read(pipefd_out, data, len);

    if (numByte < 0) { //если произошла ошибка чтения
        throw std::exception();
    }

    return numByte; //неявное преобразование, здесь numByte >= 0 всегда
}

//читает определенное количество байт с потока вывода дочернего процесса,
//соединенного через пайп; "зависнет", пока все их не считает
void Process::readExact(void* data, size_t len)
{
    size_t s = 0;
    ssize_t tempS = 0;

    while (s < len) {
        tempS = ::read(pipefd_out, (char*)data + s, len - s);

        if (tempS < 0) { //если произошла ошибка чтения
            throw std::exception();
        }

        s += tempS; //неявное преобразование, здесь numByte >= 0 всегда
    }
}

//проверка - открыт ли пайп для чтения
bool Process::isReadable() const
{
    return isReadableFlag;
}

//закрытие пайпа для записи
void Process::closeStdin()
{
    ::close(pipefd_in);
}

//закрытие пайпа для чтения
void Process::closeStdout()
{
    ::close(pipefd_out);

    isReadableFlag = false;
}

//закрытие процесса
void Process::close()
{
    kill(cpid, SIGTERM); //посылаем сигнал на выключение дочернего процесса
    waitpid(cpid, NULL, 0);
}
