#include "stderrlogger.h"

#include <iostream>

namespace log
{

StderrLogger::StderrLogger(Level lvl) noexcept : BaseLogger(lvl)
{
}

StderrLogger::~StderrLogger()
{
}

void StderrLogger::flush() noexcept
{
    std::cout.flush();
}

void StderrLogger::log_custom(const std::string &msg, Level lvl) noexcept
{
    switch(lvl) //разная кастомизация по уровням для различных логгеров
    {
    case Level::DEBUG:
    {
        std::cerr << "Debug: ";
        break;
    }

    case Level::INFO:
    {
        std::cerr << "Info: ";
        break;
    }

    case Level::WARN:
    {
        std::cerr << "Warn: ";
        break;
    }

    case Level::ERROR:
    {
        std::cerr << "Error: ";
        break;
    }
    }

    std::cerr << msg << std::endl;
}

std::unique_ptr<StderrLogger> create_stderr_logger(Level lvl)
{
    return std::make_unique<StderrLogger>(lvl);
}

}
