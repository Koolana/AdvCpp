#ifndef FILEDESCRIPTOR
#define FILEDESCRIPTOR

#include <unistd.h>

class FileDescriptor
{
public:
    FileDescriptor(int fd = -1) : _fd(fd) {}
    ~FileDescriptor() {::close(_fd);}

    int get_fd() const {return _fd;}

private:
    int _fd;
};

#endif // FILEDESCRIPTOR

