#include "logger.h"

namespace log
{

Logger& Logger::get_instance()
{
    static Logger singLogger;
    return singLogger;
}

std::shared_ptr<BaseLogger> Logger::get_global_logger() const
{
    return _globalLogger;
}

//устанавливается глобальный логгер
void Logger::set_global_logger(std::shared_ptr<BaseLogger> logger)
{
    _globalLogger = std::move(logger);
}

//при первом вызове Logger::get_instance() создается logger по умолчанию
//следовательно _globalLogger не может быть пустым, разве нет?
void debug(const std::string& msg)
{
    auto logger = Logger::get_instance().get_global_logger();
    logger->debug(msg);
}

void info(const std::string& msg)
{
    auto logger = Logger::get_instance().get_global_logger();
    logger->info(msg);
}

void warn(const std::string& msg)
{
    auto logger = Logger::get_instance().get_global_logger();
    logger->warn(msg);
}

void error(const std::string& msg)
{
    auto logger = Logger::get_instance().get_global_logger();
    logger->error(msg);
}

}//namespace log
