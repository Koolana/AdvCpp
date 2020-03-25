#include "pipe.h"

Pipe::Pipe() {
    if (pipe(fd) < 0) {
        throw std::runtime_error("Error creating pipe");
    }
}

Pipe::~Pipe() {
    close(fd[0]);
    close(fd[1]);
}

//возвращает сколько байт получилось записать
//при ошибке генерирует исключение
size_t Pipe::write(const void* data, size_t len) {
    ssize_t numByte = ::write(fd[1], data, len);

    if (numByte < 0) { //если произошла ошибка записи
        throw std::runtime_error("Error write to pipe");
    }

    return numByte; //неявное преобразование, здесь numByte >= 0 всегда
}

//возвращает сколько байт получилось считать
//при ошибке генерирует исключение
size_t Pipe::read(void* data, size_t len)
{
    ssize_t numByte = ::read(fd[0], data, len);

    if (numByte < 0) { //если произошла ошибка чтения
        throw std::runtime_error("Error read from pipe");
    }

    return numByte; //неявное преобразование, здесь numByte >= 0 всегда
}
