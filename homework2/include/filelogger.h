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

    void flush();

private:
    std::ofstream _fd;

    void log_custom(const std::string& msg) override;
};

std::unique_ptr<FileLogger> create_file_logger(const std::string& fileName, Level lvl);

}//namespace log

#endif // FILELOGGER

