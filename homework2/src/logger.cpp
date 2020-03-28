#include "logger.h"

namespace log
{

Logger& Logger::get_instance()
{
    static Logger singLogger;
    return singLogger;
}

std::shared_ptr<BaseLogger> Logger::get_global_logger() const noexcept
{
    return globalLogger;
}

//устанавливается глобальный логгер, переданный логгер больше нельзя использовать снаружи
void Logger::set_global_logger(std::unique_ptr<BaseLogger> logger) noexcept
{
    globalLogger = std::move(logger);
}

void debug(const std::string& msg) noexcept
{
    Logger::get_instance().get_global_logger()->debug(msg);
    Logger::get_instance().get_global_logger()->flush();
}

void info(const std::string& msg) noexcept
{
    Logger::get_instance().get_global_logger()->info(msg);
    Logger::get_instance().get_global_logger()->flush();
}

void warn(const std::string& msg) noexcept
{
    Logger::get_instance().get_global_logger()->warn(msg);
    Logger::get_instance().get_global_logger()->flush();
}

void error(const std::string& msg) noexcept
{
    Logger::get_instance().get_global_logger()->error(msg);
    Logger::get_instance().get_global_logger()->flush();
}

}
