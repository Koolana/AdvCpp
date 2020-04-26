#ifndef SOCKETEXCEPTION
#define SOCKETEXCEPTION

#include <exception>
#include <string>

namespace tcp
{

class SocketException : std::exception
{
public:
    SocketException();
    explicit SocketException(const std::string& str) : what_str(str) {}
    ~SocketException() = default;

    const char* what() const noexcept override { return what_str.c_str();}
private:
    std::string what_str;
};

} //namespace tcp

#endif // SOCKETEXCEPTION

