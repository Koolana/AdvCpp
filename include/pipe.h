#ifndef PIPE_H
#define PIPE_H

#include <unistd.h>
#include <stdexcept>

class Pipe
{
public:
    explicit Pipe();
    ~Pipe();

    int getPipeOutfd() const { return fd[0]; }
    int getPipeInfd() const { return fd[1]; }

    int closeOut() { return close(fd[0]); }
    int closeIn() { return close(fd[1]); }

    size_t write(const void* data, size_t len);
    size_t read(void* data, size_t len);

private:
    int fd[2];
};

#endif // PIPE_H

