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

#ifndef _EMBDB_I_LOG_SINK_HPP_
#define _EMBDB_I_LOG_SINK_HPP_

#include <string>

#include "LogLevel.hpp"

namespace embDB_logging {

class ILogSink {
 public:
  ILogSink();
  virtual ~ILogSink();

  virtual void sinkLogLine(LogLevel logLevel, const std::string& message) = 0;
};

}  // namespace embDB_logging
#endif
