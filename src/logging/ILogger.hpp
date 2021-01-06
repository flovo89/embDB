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

#ifndef _EMBDB_I_LOGGER_HPP_
#define _EMBDB_I_LOGGER_HPP_

#include "LogFormatter.hpp"

namespace embDB_logging {

class ILogger : public ILogSink {
 public:
  ILogger();
  virtual ~ILogger();

  virtual int init() = 0;
  virtual ILogger* clone(const std::string& name) = 0;
  virtual void setMaxLogLevel(LogLevel logLevel) = 0;
  virtual LogLevel getMaxLogLevel() = 0;
  virtual LogFormatter getFormater(LogLevel logLevel) = 0;
};

}  // namespace embDB_logging
#endif
