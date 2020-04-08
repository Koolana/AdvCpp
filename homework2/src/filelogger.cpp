#include "filelogger.h"

namespace log
{

FileLogger::FileLogger(const std::string& fileName, Level lvl) : BaseLogger(lvl), _fd(fileName)
{
}

void FileLogger::flush()
{
    _fd.flush();
}

void FileLogger::log_custom(const std::string& msg)
{
    _fd << msg << std::endl; //flush
}

std::unique_ptr<FileLogger> create_file_logger(const std::string& fileName, Level lvl)
{
    return std::make_unique<FileLogger>(fileName, lvl);
}

}//namespace log
