#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>
#include <iostream>

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
    void closeStdout();

    void close();
private:
    pid_t cpid;

    int pipefd_in;
    int pipefd_out;

    bool isReadableFlag;
};

#endif //PROCESS_H
