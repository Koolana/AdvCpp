#ifndef LOGGER
#define LOGGER

#include "baselogger.h"
#include <memory>

namespace log
{

class Logger
{
public:
    static Logger& get_instance();

    std::shared_ptr<BaseLogger> get_global_logger() const noexcept;
    void set_global_logger(std::unique_ptr<BaseLogger> logger) noexcept;

private:
    Logger(){}
    Logger(const Logger& root) = delete;
    Logger& operator=(const Logger&) = delete;

    std::shared_ptr<BaseLogger> globalLogger;
};

void debug(const std::string& msg) noexcept;
void info(const std::string& msg) noexcept;
void warn(const std::string& msg) noexcept;
void error(const std::string& msg) noexcept;

}

#endif // LOGGER

