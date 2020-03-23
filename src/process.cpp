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
        throw std::runtime_error("Error creating pipe in");
    }

    if (pipe(pipefdOut) == -1) {
        ::close(pipefdIn[0]); //закрытие первой пайпы,
        ::close(pipefdIn[1]); //если создание второй провалилось

        throw std::runtime_error("Error creating pipe out");
    }

    cpid = fork();
    if (cpid == -1) {
        ::close(pipefdIn[0]); //закрытие первой пайпы,
        ::close(pipefdIn[1]); //если fork() провалился

        ::close(pipefdOut[0]); //закрытие второй пайпы,
        ::close(pipefdOut[1]); //если fork() провалился

        throw std::runtime_error("Error fork()");
    }

    if (cpid == 0) { //Child
        ::close(pipefdIn[1]);
        ::close(pipefdOut[0]);

        dup2(pipefdIn[0], STDIN_FILENO);
        dup2(pipefdOut[1], STDOUT_FILENO);

        if (execl(path.data(), path.data()) == -1) {
            throw std::runtime_error("Error execl() in child");
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
    close();
}

//пишет определенное количество байт в поток ввода дочернего процесса,
//соединенного через пайп;
size_t Process::write(const void* data, size_t len)
{
    ssize_t numByte = ::write(pipefd_in, data, len);

    if (numByte < 0) { //если произошла ошибка записи
        throw std::runtime_error("Error write");
    }

    return numByte; //неявное преобразование, здесь numByte >= 0 всегда
}

//пишет определенное количество байт в поток ввода дочернего процесса,
//соединенного через пайп; "зависнет", пока все не запишет
void Process::writeExact(const void* data, size_t len)
{
    size_t s = 0;
    ssize_t numByte = 0;

    while (s < len) {
        numByte = ::write(pipefd_in, (char*)data + s, len - s);

        if (numByte < 0) { //если произошла ошибка записи
            throw std::runtime_error("Error writeExact");
        }

        s += numByte; //неявное преобразование, здесь numByte >= 0 всегда
    }
}

//читает определенное количество байт с потока вывода дочернего процесса,
//соединенного через пайп;
size_t Process::read(void* data, size_t len)
{
    ssize_t numByte = ::read(pipefd_out, data, len);

    if (numByte < 0) { //если произошла ошибка чтения
        throw std::runtime_error("Error read");
    }

    return numByte; //неявное преобразование, здесь numByte >= 0 всегда
}

//читает определенное количество байт с потока вывода дочернего процесса,
//соединенного через пайп; "зависнет", пока все их не считает
void Process::readExact(void* data, size_t len)
{
    size_t s = 0;
    ssize_t numByte = 0;

    while (s < len) {
        numByte = ::read(pipefd_out, (char*)data + s, len - s);

        if (numByte < 0) { //если произошла ошибка чтения
            throw std::runtime_error("Error readExact");
        }

        s += numByte; //неявное преобразование, здесь tempS >= 0 всегда
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

//закрытие процесса
void Process::close()
{
    isReadableFlag = false;

    ::close(pipefd_in);
    ::close(pipefd_out);

    kill(cpid, SIGTERM); //посылаем сигнал на выключение дочернего процесса
    waitpid(cpid, NULL, 0);
}
