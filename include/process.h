#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>
#include <iostream>
#include "pipe.h"

class Process
{
public:
    explicit Process(const std::string& path);
    ~Process();

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);

    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    bool isReadable() const;

    void closeStdin();

    void close();
private:
    pid_t cpid;

    Pipe pipe2read;  //создает пайп конструктором по умолчанию,
    Pipe pipe2write; //при выходе из зоны видимости обеспечивает его закрытие

    bool isReadableFlag;
};

#endif //PROCESS_H
