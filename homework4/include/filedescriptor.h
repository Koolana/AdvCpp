#ifndef FILEDESCRIPTOR
#define FILEDESCRIPTOR

#include <unistd.h>
#include <utility>

class FileDescriptor
{
public:
    FileDescriptor(int fd = -1) : _fd(fd) {}
    ~FileDescriptor() {::close(_fd);}

    int extract() {return std::exchange(_fd, -1);}
    int get_fd() const {return _fd;}

private:
    int _fd;
};

#endif // FILEDESCRIPTOR

