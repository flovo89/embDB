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

#include "LoggerFactory.hpp"

#include <unistd.h>

#include "LoggerConfig.hpp"

#if EMBDB_HAS_LOG4CPLUS == 1
#include "log4cplus/LoggerLog4Cplus.hpp"
#else
#include "console/LoggerConsole.hpp"
#include "null/LoggerNull.hpp"
#endif

namespace embDB_logging {

//--------------------------------------------------------------------------------------------
int LoggerFactory::createDefaultLogger(std::unique_ptr<ILogger>& logger,
                                       const std::string& config) {
#if EMBDB_HAS_LOG4CPLUS == 1
  logger.reset(new LoggerLog4Cplus(config));
#else
  // Use console logger if configuration file is found
  if (access(config.c_str(), F_OK))
    logger.reset(new LoggerNull());
  else
    logger.reset(new LoggerConsole());
#endif

  return 0;
}

}  // namespace embDB_logging
