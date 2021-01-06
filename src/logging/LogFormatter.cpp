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

#include "LogFormatter.hpp"

namespace embDB_logging {

//--------------------------------------------------------------------------------------------
LogFormatter::LogFormatter(LogLevel logLevel, ILogSink& logWriter)
    : m_logLevel(logLevel), m_logSink(logWriter), m_isEnabled(true) {}

//--------------------------------------------------------------------------------------------
LogFormatter::LogFormatter(ILogSink& logSink)
    : m_logLevel(LogLevel::DEBUG), m_logSink(logSink), m_isEnabled(false) {}

//--------------------------------------------------------------------------------------------
LogFormatter::~LogFormatter() {
  if (m_isEnabled) m_logSink.sinkLogLine(m_logLevel, m_ss.str());
}

//--------------------------------------------------------------------------------------------
std::ostream& LogFormatter::get() {
  if (!m_isEnabled) m_ss.setstate(std::ios_base::badbit);

  return m_ss;
}

}  // namespace embDB_logging
