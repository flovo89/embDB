/*
 * embDB
 * Copyright (C) 2021 flovo89
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LoggerLog4Cplus.hpp"

#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>

#include "CustomLog4CplusLogsLevels.hpp"

namespace embDB_logging {

//--------------------------------------------------------------------------------------------
LoggerLog4Cplus::LoggerLog4Cplus() {}

//--------------------------------------------------------------------------------------------
LoggerLog4Cplus::LoggerLog4Cplus(const std::string& configFile)
    : m_configFile(configFile) {}

//--------------------------------------------------------------------------------------------
LoggerLog4Cplus::LoggerLog4Cplus(const log4cplus::Logger& logger)
    : m_log4CplusLogger(logger) {}

//--------------------------------------------------------------------------------------------
LoggerLog4Cplus::~LoggerLog4Cplus() {}

//--------------------------------------------------------------------------------------------
int LoggerLog4Cplus::init() {
  log4cplus::initialize();

  if (m_configFile != "")
    log4cplus::PropertyConfigurator::doConfigure(m_configFile);

  log4cplus::getLogLevelManager().pushToStringMethod(
      noteLogLevelToStringMethod);
  log4cplus::getLogLevelManager().pushToStringMethod(
      criticalLogLevelToStringMethod);
  log4cplus::getLogLevelManager().pushToStringMethod(
      alertLogLevelToStringMethod);
  log4cplus::getLogLevelManager().pushToStringMethod(
      emergencyLogLevelToStringMethod);
  log4cplus::getLogLevelManager().pushFromStringMethod(
      noteStringToLogLevelMethod);
  log4cplus::getLogLevelManager().pushFromStringMethod(
      criticalStringToLogLevelMethod);
  log4cplus::getLogLevelManager().pushFromStringMethod(
      alertStringToLogLevelMethod);
  log4cplus::getLogLevelManager().pushFromStringMethod(
      emergencyStringToLogLevelMethod);

  return 0;
}

//--------------------------------------------------------------------------------------------
ILogger* LoggerLog4Cplus::clone(const std::string& name) {
  log4cplus::Logger l;

  // Make sure root logger has an appender, otherwise use default console
  // appender
  if (log4cplus::Logger::getRoot().getAllAppenders().size() == 0) {
    log4cplus::SharedAppenderPtr defaultAppender(
        new log4cplus::ConsoleAppender());
    log4cplus::Logger::getRoot().addAppender(defaultAppender);
  }

  if (name == "")
    l = log4cplus::Logger::getRoot();
  else
    l = log4cplus::Logger::getInstance(name);

  return new LoggerLog4Cplus(l);
}

//--------------------------------------------------------------------------------------------
void LoggerLog4Cplus::sinkLogLine(LogLevel logLevel,
                                  const std::string& message) {
  if (logLevel == LogLevel::EMERGENCY) {
    LOG4CPLUS_EMERGENCY(m_log4CplusLogger, message);
  } else if (logLevel == LogLevel::ALERT) {
    LOG4CPLUS_ALERT(m_log4CplusLogger, message);
  } else if (logLevel == LogLevel::CRITICAL) {
    LOG4CPLUS_CRITICAL(m_log4CplusLogger, message);
  } else if (logLevel == LogLevel::ERROR) {
    LOG4CPLUS_ERROR(m_log4CplusLogger, message);
  } else if (logLevel == LogLevel::WARNING) {
    LOG4CPLUS_WARN(m_log4CplusLogger, message);
  } else if (logLevel == LogLevel::NOTE) {
    LOG4CPLUS_NOTE(m_log4CplusLogger, message);
  } else if (logLevel == LogLevel::INFO) {
    LOG4CPLUS_INFO(m_log4CplusLogger, message);
  } else {
    LOG4CPLUS_DEBUG(m_log4CplusLogger, message);
  }
}

}  // namespace embDB_logging
