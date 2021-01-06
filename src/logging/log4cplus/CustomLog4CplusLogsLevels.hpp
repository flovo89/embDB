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

#ifndef _EMBDB_CUSTOM_LOG4CPLUS_LOGLEVELS_HPP_
#define _EMBDB_CUSTOM_LOG4CPLUS_LOGLEVELS_HPP_

#include <log4cplus/loglevel.h>

namespace log4cplus {

const LogLevel NOTE_LOG_LEVEL = INFO_LOG_LEVEL + 500;
const LogLevel CRITICAL_LOG_LEVEL = ERROR_LOG_LEVEL + 500;
const LogLevel ALERT_LOG_LEVEL = CRITICAL_LOG_LEVEL + 500;
const LogLevel EMERGENCY_LOG_LEVEL = ALERT_LOG_LEVEL + 500;

}  // namespace log4cplus

#define LOG4CPLUS_NOTE(logger, logEvent)                              \
  if (logger.isEnabledFor(log4cplus::NOTE_LOG_LEVEL)) {               \
    log4cplus::tostringstream _log4cplus_buf;                         \
    _log4cplus_buf << logEvent;                                       \
    logger.forcedLog(log4cplus::NOTE_LOG_LEVEL, _log4cplus_buf.str(), \
                     __FILE__, __LINE__);                             \
  }

#define LOG4CPLUS_CRITICAL(logger, logEvent)                              \
  if (logger.isEnabledFor(log4cplus::CRITICAL_LOG_LEVEL)) {               \
    log4cplus::tostringstream _log4cplus_buf;                             \
    _log4cplus_buf << logEvent;                                           \
    logger.forcedLog(log4cplus::CRITICAL_LOG_LEVEL, _log4cplus_buf.str(), \
                     __FILE__, __LINE__);                                 \
  }

#define LOG4CPLUS_ALERT(logger, logEvent)                              \
  if (logger.isEnabledFor(log4cplus::ALERT_LOG_LEVEL)) {               \
    log4cplus::tostringstream _log4cplus_buf;                          \
    _log4cplus_buf << logEvent;                                        \
    logger.forcedLog(log4cplus::ALERT_LOG_LEVEL, _log4cplus_buf.str(), \
                     __FILE__, __LINE__);                              \
  }

#define LOG4CPLUS_EMERGENCY(logger, logEvent)                              \
  if (logger.isEnabledFor(log4cplus::EMERGENCY_LOG_LEVEL)) {               \
    log4cplus::tostringstream _log4cplus_buf;                              \
    _log4cplus_buf << logEvent;                                            \
    logger.forcedLog(log4cplus::EMERGENCY_LOG_LEVEL, _log4cplus_buf.str(), \
                     __FILE__, __LINE__);                                  \
  }

namespace embDB_logging {

log4cplus::tstring const& noteLogLevelToStringMethod(log4cplus::LogLevel ll);
log4cplus::LogLevel noteStringToLogLevelMethod(const log4cplus::tstring& s);
log4cplus::tstring const& criticalLogLevelToStringMethod(
    log4cplus::LogLevel ll);
log4cplus::LogLevel criticalStringToLogLevelMethod(const log4cplus::tstring& s);
log4cplus::tstring const& alertLogLevelToStringMethod(log4cplus::LogLevel ll);
log4cplus::LogLevel alertStringToLogLevelMethod(const log4cplus::tstring& s);
log4cplus::tstring const& emergencyLogLevelToStringMethod(
    log4cplus::LogLevel ll);
log4cplus::LogLevel emergencyStringToLogLevelMethod(
    const log4cplus::tstring& s);

}  // namespace embDB_logging
#endif
