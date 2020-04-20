#ifndef BASELOGGER
#define BASELOGGER

#include <memory>
#include <string>
#include <map>

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
    explicit BaseLogger(Level lvl);
    virtual ~BaseLogger() = default;

    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);

    void set_level(Level lvl);
    Level level() const;

    virtual void flush() = 0;

private:
    Level _level;

    const std::string& getPrefix(Level lvl);

    void log(const std::string& msg, Level lvl);
    virtual void log_custom(const std::string& msg) = 0;
};

} //namespace log

#endif // BASELOGGER

