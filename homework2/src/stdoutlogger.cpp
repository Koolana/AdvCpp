#include "stdoutlogger.h"

#include <iostream>

namespace log
{

StdoutLogger::StdoutLogger(Level lvl) : BaseLogger(lvl)
{
}

void StdoutLogger::flush()
{
    std::cout.flush();
}

void StdoutLogger::log_custom(const std::string &msg)
{
    std::cout << msg << std::endl; //flush
}

std::unique_ptr<StdoutLogger> create_stdout_logger(Level lvl)
{
    return std::make_unique<StdoutLogger>(lvl);
}

}//namespace log
