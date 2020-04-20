#include "stderrlogger.h"

#include <iostream>

namespace log
{

StderrLogger::StderrLogger(Level lvl) : BaseLogger(lvl)
{
}

void StderrLogger::flush()
{
    std::cout.flush();
}

void StderrLogger::log_custom(const std::string &msg)
{
    std::cerr << msg << std::endl; //flush
}

std::unique_ptr<StderrLogger> create_stderr_logger(Level lvl)
{
    return std::make_unique<StderrLogger>(lvl);
}

}//namespace log
