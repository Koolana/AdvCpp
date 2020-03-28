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
    cpid = fork();
    if (cpid == -1) {
        throw std::runtime_error("Error fork()");
    }

    if (cpid == 0) { //Child
        pipe2write.closeIn(); //закрытие неиспользуемых концов пайп
        pipe2read.closeOut(); //чтобы их случайно не использовать

        dup2(pipe2write.getPipeOutfd(), STDIN_FILENO);
        dup2(pipe2read.getPipeInfd(), STDOUT_FILENO);

        if (execl(path.data(), path.data()) == -1) {
            throw std::runtime_error("Error execl() in child");
        }
    }
    else { //Parent
        pipe2write.closeOut(); //закрытие неиспользуемых концов пайп
        pipe2read.closeIn(); //чтобы их случайно не использовать

        isReadableFlag = true;
    }
}

Process::~Process()
{
    close();
}

//пишет определенное количество байт в поток ввода дочернего процесса,
//соединенного через pipe2write;
size_t Process::write(const void* data, size_t len)
{
    return pipe2write.write(data, len);
}

//пишет определенное количество байт в поток ввода дочернего процесса,
//соединенного через pipe2write; "зависнет", пока все не запишет
void Process::writeExact(const void* data, size_t len)
{
    size_t s = 0;

    while (s < len) {
        s += pipe2write.write((char*)data + s, len - s);
    }
}

//читает определенное количество байт с потока вывода дочернего процесса,
//соединенного через pipe2read;
size_t Process::read(void* data, size_t len)
{
    return pipe2read.read(data, len);
}

//читает определенное количество байт с потока вывода дочернего процесса,
//соединенного через pipe2read; "зависнет", пока все их не считает
void Process::readExact(void* data, size_t len)
{
    size_t s = 0;

    while (s < len) {
        s += pipe2read.read((char*)data + s, len - s);
    }
}

//проверка - возможно ли чтение
bool Process::isReadable() const
{
    return isReadableFlag;
}

//закрытие пайпа для записи
void Process::closeStdin()
{
    pipe2write.closeIn();
}

//закрытие процесса
void Process::close()
{
    pipe2write.closeIn(); //закрытие всех пайп, ведущих к дочернему процессу
    pipe2read.closeOut(); //перед его завершением

    isReadableFlag = false;

    kill(cpid, SIGTERM); //посылаем сигнал на выключение дочернего процесса
    waitpid(cpid, NULL, 0); //ждем выключения дочернего процесса
}
