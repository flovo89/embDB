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

#include "CustomLog4CplusLogsLevels.hpp"

static log4cplus::tstring const NOTE_STRING(LOG4CPLUS_TEXT("NOTE"));
static log4cplus::tstring const CRITICAL_STRING(LOG4CPLUS_TEXT("CRITICAL"));
static log4cplus::tstring const ALERT_STRING(LOG4CPLUS_TEXT("ALERT"));
static log4cplus::tstring const EMERGENCY_STRING(LOG4CPLUS_TEXT("EMERGENCY"));
static log4cplus::tstring const empty_str;

namespace embDB_logging {

//--------------------------------------------------------------------------------------------
log4cplus::tstring const& noteLogLevelToStringMethod(log4cplus::LogLevel ll) {
  if (ll == log4cplus::NOTE_LOG_LEVEL) return NOTE_STRING;
  return empty_str;
}

//--------------------------------------------------------------------------------------------
log4cplus::LogLevel noteStringToLogLevelMethod(const log4cplus::tstring& s) {
  if (s == NOTE_STRING) return log4cplus::NOTE_LOG_LEVEL;
  return log4cplus::NOT_SET_LOG_LEVEL;
}

//--------------------------------------------------------------------------------------------
log4cplus::tstring const& criticalLogLevelToStringMethod(
    log4cplus::LogLevel ll) {
  if (ll == log4cplus::CRITICAL_LOG_LEVEL) return CRITICAL_STRING;
  return empty_str;
}

//--------------------------------------------------------------------------------------------
log4cplus::LogLevel criticalStringToLogLevelMethod(
    const log4cplus::tstring& s) {
  if (s == CRITICAL_STRING) return log4cplus::CRITICAL_LOG_LEVEL;
  return log4cplus::NOT_SET_LOG_LEVEL;
}

//--------------------------------------------------------------------------------------------
log4cplus::tstring const& alertLogLevelToStringMethod(log4cplus::LogLevel ll) {
  if (ll == log4cplus::ALERT_LOG_LEVEL) return ALERT_STRING;
  return empty_str;
}

//--------------------------------------------------------------------------------------------
log4cplus::LogLevel alertStringToLogLevelMethod(const log4cplus::tstring& s) {
  if (s == ALERT_STRING) return log4cplus::ALERT_LOG_LEVEL;
  return log4cplus::NOT_SET_LOG_LEVEL;
}

//--------------------------------------------------------------------------------------------
log4cplus::tstring const& emergencyLogLevelToStringMethod(
    log4cplus::LogLevel ll) {
  if (ll == log4cplus::EMERGENCY_LOG_LEVEL) return EMERGENCY_STRING;
  return empty_str;
}

//--------------------------------------------------------------------------------------------
log4cplus::LogLevel emergencyStringToLogLevelMethod(
    const log4cplus::tstring& s) {
  if (s == EMERGENCY_STRING) return log4cplus::EMERGENCY_LOG_LEVEL;
  return log4cplus::NOT_SET_LOG_LEVEL;
}

}  // namespace embDB_logging
