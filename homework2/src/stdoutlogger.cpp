#include "stdoutlogger.h"

#include <iostream>

namespace log
{

StdoutLogger::StdoutLogger(Level lvl) noexcept : BaseLogger(lvl)
{
}

StdoutLogger::~StdoutLogger()
{
}

void StdoutLogger::flush() noexcept
{
    std::cout.flush();
}

void StdoutLogger::log_custom(const std::string &msg, Level lvl) noexcept
{
    switch(lvl) //возможна разная кастомизация для различных логгеров
    {
    case Level::DEBUG:
    {
        std::cout << "Debug: ";
        break;
    }

    case Level::INFO:
    {
        std::cout << "Info: ";
        break;
    }

    case Level::WARN:
    {
        std::cout << "Warn: ";
        break;
    }

    case Level::ERROR:
    {
        std::cout << "Error: ";
        break;
    }
    }

    std::cout << msg << std::endl;
}

std::unique_ptr<StdoutLogger> create_stdout_logger(Level lvl)
{
    return std::make_unique<StdoutLogger>(lvl);
}

}
