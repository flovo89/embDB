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

#ifndef _EMBDB_LOGGERS_HPP_
#define _EMBDB_LOGGERS_HPP_

#include <memory>
#include <unordered_map>

#include "../utilities/IMutex.hpp"
#include "ILogger.hpp"
#include "null/LoggerNull.hpp"

namespace embDB_logging {

class Loggers {
 public:
  Loggers(Loggers const&) = delete;
  Loggers(Loggers&&) = delete;
  Loggers& operator=(Loggers const&) = delete;
  Loggers& operator=(Loggers&&) = delete;

  static Loggers& getInstance();
  void setDefaultLogger(std::unique_ptr<ILogger>& defaultLogger);
  int initDefault(const std::string& config, bool isThreaded = false);
  int init(bool isThreaded = false);
  ILogger& getLogger(const std::string& which);
  void setMaxLogLevel(LogLevel level);

 private:
  typedef std::unordered_map<std::string, std::unique_ptr<ILogger> > LoggersMap;
  LoggersMap m_loggers;
  std::unique_ptr<ILogger> m_default;
  std::unique_ptr<embDB_utilities::IMutex> m_mtx;

  Loggers();
  ~Loggers();
};

}  // namespace embDB_logging

// Logging helper macros
#define _LOG(cat, loglevel)             \
  embDB_logging::Loggers::getInstance() \
      .getLogger(cat)                   \
      .getFormater(loglevel)            \
      .get()

#define LOG_CAT(cat) _LOG(cat, embDB_logging::LogLevel::INFO)
#define LOG_CAT_EM(cat) _LOG(cat, embDB_logging::LogLevel::EMERGENCY)
#define LOG_CAT_AL(cat) _LOG(cat, embDB_logging::LogLevel::ALERT)
#define LOG_CAT_CR(cat) _LOG(cat, embDB_logging::LogLevel::CRITICAL)
#define LOG_CAT_ER(cat) _LOG(cat, embDB_logging::LogLevel::ERROR)
#define LOG_CAT_WA(cat) _LOG(cat, embDB_logging::LogLevel::WARNING)
#define LOG_CAT_NO(cat) _LOG(cat, embDB_logging::LogLevel::NOTE)
#define LOG_CAT_IN(cat) _LOG(cat, embDB_logging::LogLevel::INFO)
#define LOG_CAT_DB(cat) _LOG(cat, embDB_logging::LogLevel::DEBUG)

#define LOG() LOG_CAT("")
#define LOG_EM() LOG_CAT_EM("")
#define LOG_AL() LOG_CAT_AL("")
#define LOG_CR() LOG_CAT_CR("")
#define LOG_ER() LOG_CAT_ER("")
#define LOG_WA() LOG_CAT_WA("")
#define LOG_NO() LOG_CAT_NO("")
#define LOG_IN() LOG_CAT_IN("")
#define LOG_DB() LOG_CAT_DB("")

#endif
