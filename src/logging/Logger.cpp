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

#include "Logger.hpp"

namespace embDB_logging {

//--------------------------------------------------------------------------------------------
Logger::Logger() : m_maxLevel(LogLevel::DEBUG) {}

//--------------------------------------------------------------------------------------------
Logger::Logger(LogLevel maxLevel) : m_maxLevel(maxLevel) {}

//--------------------------------------------------------------------------------------------
Logger::~Logger() {}

//--------------------------------------------------------------------------------------------
int Logger::init() { return 0; }

//--------------------------------------------------------------------------------------------
LogFormatter Logger::getFormater(LogLevel logLevel) {
  if (logLevel <= m_maxLevel)
    return LogFormatter(logLevel, *this);
  else
    return LogFormatter(*this);
}

//--------------------------------------------------------------------------------------------
void Logger::setMaxLogLevel(LogLevel logLevel) { m_maxLevel = logLevel; }

//--------------------------------------------------------------------------------------------
LogLevel Logger::getMaxLogLevel() { return m_maxLevel; }

}  // namespace embDB_logging
