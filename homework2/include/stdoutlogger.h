#ifndef STDOUTLOGGER
#define STDOUTLOGGER

#include "baselogger.h"

namespace log
{

class StdoutLogger: public BaseLogger
{
public:
    explicit StdoutLogger(Level lvl) noexcept;
    ~StdoutLogger();

    void flush() noexcept;

private:
    void log_custom(const std::string& msg, Level lvl) noexcept;
};

std::unique_ptr<StdoutLogger> create_stdout_logger(Level lvl);

}

#endif // STDOUTLOGGER

