#include "baselogger.h"

#include <iostream>

namespace log
{

BaseLogger::BaseLogger(Level lvl) noexcept
{
    set_level(lvl);
}

BaseLogger::~BaseLogger()
{
}

//вызов лог с уровнем DEBUG
void BaseLogger::debug(const std::string& msg) noexcept
{
    log(msg, Level::DEBUG);
}

//вызов лог с уровнем INFO
void BaseLogger::info(const std::string& msg) noexcept
{
    log(msg, Level::INFO);
}

//вызов лог с уровнем WARNING
void BaseLogger::warn(const std::string& msg) noexcept
{
    log(msg, Level::WARN);
}

//вызов лог с уровнем ERROR
void BaseLogger::error(const std::string& msg) noexcept
{
    log(msg, Level::ERROR);
}

void BaseLogger::set_level(Level lvl) noexcept
{
    level_ = lvl;
}

Level BaseLogger::level() noexcept
{
    return level_;
}

//вызов кастомизированного лога с корректным уровнем
void BaseLogger::log(const std::string& msg, Level lvl) noexcept
{
    if (lvl >= level_) {
        log_custom(msg, lvl);
    }
}

}//namespace log
