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

#ifndef _EMBDB_LOGGER_HPP_
#define _EMBDB_LOGGER_HPP_

#include "ILogger.hpp"

namespace embDB_logging {

class Logger : public ILogger {
 public:
  Logger();
  Logger(LogLevel maxLevel);
  virtual ~Logger();

  virtual int init() override;
  virtual LogFormatter getFormater(LogLevel logLevel) override;
  virtual void setMaxLogLevel(LogLevel logLevel) override;
  virtual LogLevel getMaxLogLevel() override;

 protected:
  LogLevel m_maxLevel;
};

}  // namespace embDB_logging
#endif
