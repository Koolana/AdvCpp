#ifndef STDOUTLOGGER
#define STDOUTLOGGER

#include "baselogger.h"

namespace log
{

class StdoutLogger: public BaseLogger
{
public:
    explicit StdoutLogger(Level lvl);

    void flush();

private:
    void log_custom(const std::string& msg) override;
};

std::unique_ptr<StdoutLogger> create_stdout_logger(Level lvl);

}//namespace log

#endif // STDOUTLOGGER

