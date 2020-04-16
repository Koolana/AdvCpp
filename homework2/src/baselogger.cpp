#include "baselogger.h"

#include <iostream>

namespace log
{

BaseLogger::BaseLogger(Level lvl)
{
    set_level(lvl);
}

//вызов лог с уровнем DEBUG
void BaseLogger::debug(const std::string& msg)
{
    log(msg, Level::DEBUG);
}

//вызов лог с уровнем INFO
void BaseLogger::info(const std::string& msg)
{
    log(msg, Level::INFO);
}

//вызов лог с уровнем WARNING
void BaseLogger::warn(const std::string& msg)
{
    log(msg, Level::WARN);
}

//вызов лог с уровнем ERROR
void BaseLogger::error(const std::string& msg)
{
    log(msg, Level::ERROR);
}

void BaseLogger::set_level(Level lvl)
{
    _level = lvl;
}

Level BaseLogger::level() const
{
    return _level;
}

const std::string& BaseLogger::getPrefix(Level lvl)
{
    const std::map<Level, std::string> LOG_LEVEL_PREFIX{
        {Level::DEBUG, "Debug: "},
        {Level::INFO, "Info: "},
        {Level::WARN, "Warn: "},
        {Level::ERROR, "Error: "}
    };

    return LOG_LEVEL_PREFIX.at(lvl);
}

//вызов кастомизированного лога с корректным уровнем
void BaseLogger::log(const std::string& msg, Level lvl)
{
    if (lvl >= _level) {
        log_custom(getPrefix(lvl) + msg);
    }
}

}//namespace log
