#ifndef STDERRLOGGER
#define STDERRLOGGER

#include "baselogger.h"

namespace log
{

class StderrLogger: public BaseLogger
{
public:
    explicit StderrLogger(Level lvl) noexcept;
    ~StderrLogger();

    void flush() noexcept;

private:
    void log_custom(const std::string& msg, Level lvl) noexcept;
};

std::unique_ptr<StderrLogger> create_stderr_logger(Level lvl);

}

#endif // STDERRLOGGER

