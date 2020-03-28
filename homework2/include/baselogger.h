#ifndef BASELOGGER
#define BASELOGGER

#include <memory>
#include <string>

namespace log
{

enum class Level{
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class BaseLogger
{
public:
    explicit BaseLogger(Level lvl) noexcept;
    virtual ~BaseLogger() = 0;

    void debug(const std::string& msg) noexcept;
    void info(const std::string& msg) noexcept;
    void warn(const std::string& msg) noexcept;
    void error(const std::string& msg) noexcept;

    void set_level(Level lvl) noexcept;
    Level level() noexcept;

    virtual void flush() noexcept = 0;

private:
    Level level_;

    void log(const std::string& msg, Level lvl) noexcept;
    virtual void log_custom(const std::string& msg, Level lvl) noexcept = 0;
};

} //namespace log

#endif // BASELOGGER

