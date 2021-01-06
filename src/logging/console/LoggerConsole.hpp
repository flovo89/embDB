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

#ifndef _EMBDB_LOGGER_CONSOLE_HPP_
#define _EMBDB_LOGGER_CONSOLE_HPP_

#include "../Logger.hpp"

namespace embDB_logging {

class LoggerConsole : public Logger {
 public:
  LoggerConsole();
  LoggerConsole(const std::string& name);
  LoggerConsole(const std::string& name, LogLevel maxLogLevel);
  virtual ~LoggerConsole();

  virtual ILogger* clone(const std::string& name) override;
  virtual void sinkLogLine(LogLevel logLevel,
                           const std::string& message) override;

 private:
  std::string m_name;
};

}  // namespace embDB_logging
#endif
