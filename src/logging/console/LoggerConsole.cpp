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

#include "LoggerConsole.hpp"

#include <iostream>

namespace embDB_logging {

//--------------------------------------------------------------------------------------------
LoggerConsole::LoggerConsole() {}

//--------------------------------------------------------------------------------------------
LoggerConsole::LoggerConsole(const std::string& name) : m_name(name) {}

//--------------------------------------------------------------------------------------------
LoggerConsole::LoggerConsole(const std::string& name, LogLevel maxLogLevel)
    : Logger(maxLogLevel), m_name(name) {}

//--------------------------------------------------------------------------------------------
LoggerConsole::~LoggerConsole() {}

//--------------------------------------------------------------------------------------------
ILogger* LoggerConsole::clone(const std::string& name) {
  return new LoggerConsole(name, m_maxLevel);
}

//--------------------------------------------------------------------------------------------
void LoggerConsole::sinkLogLine(LogLevel logLevel, const std::string& message) {
  if (logLevel == LogLevel::DEBUG)
    std::cout << "[DEBUG]      ";
  else if (logLevel == LogLevel::INFO)
    std::cout << "[INFO]       ";
  else if (logLevel == LogLevel::NOTE)
    std::cout << "[NOTE]       ";
  else if (logLevel == LogLevel::WARNING)
    std::cout << "[WARNING]    ";
  else if (logLevel == LogLevel::ERROR)
    std::cout << "[ERROR]      ";
  else if (logLevel == LogLevel::CRITICAL)
    std::cout << "[CRITICAL]   ";
  else if (logLevel == LogLevel::ALERT)
    std::cout << "[ALERT]      ";
  else if (logLevel == LogLevel::EMERGENCY)
    std::cout << "[EMERGENCY]  ";
  else
    std::cout << "[UNKOWN]     ";

  if (m_name != "") std::cout << m_name << ": ";

  std::cout << message << std::endl;
}

}  // namespace embDB_logging
