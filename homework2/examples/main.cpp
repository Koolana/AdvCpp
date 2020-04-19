#include <iostream>
#include <memory>
#include "logger.h"
#include "stdoutlogger.h"
#include "stderrlogger.h"
#include "filelogger.h"

int main()
{
    log::StdoutLogger stdoutLog(log::Level::DEBUG);
    stdoutLog.debug("Test message - Debug");
    stdoutLog.info("Test message - Info");
    stdoutLog.warn("Test message - Warning");
    stdoutLog.error("Test message - Error");

    //log::Logger::get_instance().set_global_logger(log::create_stderr_logger(log::Level::WARN));
    log::debug("Singleton debug");
    log::info("Singleton info");
    log::warn("Singleton warning");
    log::error("Singleton error");

    log::Logger::get_instance().set_global_logger(NULL);
    log::debug("Singleton debug");
    log::info("Singleton info");
    log::warn("Singleton warning");
    log::error("Singleton error");

    return 0;
}

