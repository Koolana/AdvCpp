#ifndef FILELOGGER
#define FILELOGGER

#include "baselogger.h"
#include <fstream>

namespace log
{

class FileLogger: public BaseLogger
{
public:
    explicit FileLogger(const std::string& fileName, Level lvl);
    ~FileLogger();

    void flush() noexcept;

private:
    std::ofstream fd;

    void log_custom(const std::string& msg, Level lvl) noexcept;
};

std::unique_ptr<FileLogger> create_file_logger(const std::string& fileName, Level lvl);
}

#endif // FILELOGGER

