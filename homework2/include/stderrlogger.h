#ifndef STDERRLOGGER
#define STDERRLOGGER

#include "baselogger.h"

namespace log
{

class StderrLogger: public BaseLogger
{
public:
    explicit StderrLogger(Level lvl);

    void flush();

private:
    void log_custom(const std::string& msg) override;
};

std::unique_ptr<StderrLogger> create_stderr_logger(Level lvl);

}//namespace log

#endif // STDERRLOGGER

