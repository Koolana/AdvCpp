#include "filelogger.h"

namespace log
{

FileLogger::FileLogger(const std::string& fileName, Level lvl) : BaseLogger(lvl), fd(fileName)
{
}

FileLogger::~FileLogger()
{
}

void FileLogger::flush() noexcept
{
    fd.flush();
}

void FileLogger::log_custom(const std::string& msg, Level lvl) noexcept
{
    switch(lvl) //возможна разная кастомизация для различных логгеров
    {
    case Level::DEBUG:
    {
        fd << "Debug: ";
        break;
    }

    case Level::INFO:
    {
        fd << "Info: ";
        break;
    }

    case Level::WARN:
    {
        fd << "Warn: ";
        break;
    }

    case Level::ERROR:
    {
        fd << "Error: ";
        break;
    }
    }

    fd << msg << std::endl;
}

std::unique_ptr<FileLogger> create_file_logger(const std::string& fileName, Level lvl)
{
    return std::make_unique<FileLogger>(fileName, lvl);
}

}
