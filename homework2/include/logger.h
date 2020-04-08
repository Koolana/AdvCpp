#ifndef LOGGER
#define LOGGER

#include "stderrlogger.h"
#include <memory>

namespace log
{

class Logger
{
public:
    static Logger& get_instance();

    std::shared_ptr<BaseLogger> get_global_logger() const;
    void set_global_logger(std::shared_ptr<BaseLogger> logger);

private:
    Logger() {set_global_logger(log::create_stderr_logger(log::Level::DEBUG));} //инициализируем по умолчанию stderr_logger'ом
    Logger(const Logger& root) = delete;
    Logger& operator=(const Logger&) = delete;

    std::shared_ptr<BaseLogger> _globalLogger;
};

void debug(const std::string& msg);
void info(const std::string& msg);
void warn(const std::string& msg);
void error(const std::string& msg);

}//namespace log

#endif // LOGGER

