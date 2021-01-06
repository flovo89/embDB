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

#include "Loggers.hpp"

#include <iostream>

#include "../exception/Exception.hpp"
#include "../utilities/DefaultMutex.hpp"
#include "../utilities/NullMutex.hpp"
#include "LoggerFactory.hpp"
#include "console/LoggerConsole.hpp"

namespace embDB_logging {

//--------------------------------------------------------------------------------------------
Loggers::Loggers()
    : m_default(new LoggerNull()), m_mtx(new embDB_utilities::NullMutex()) {}

//--------------------------------------------------------------------------------------------
Loggers::~Loggers() {}

//--------------------------------------------------------------------------------------------
Loggers& Loggers::getInstance() {
  static Loggers loggers;
  return loggers;
}

//--------------------------------------------------------------------------------------------
void Loggers::setDefaultLogger(std::unique_ptr<ILogger>& defaultLogger) {
  m_default = std::move(defaultLogger);
}

//--------------------------------------------------------------------------------------------
int Loggers::initDefault(const std::string& config, bool isThreaded) {
  if (LoggerFactory::createDefaultLogger(m_default, config)) return -1;
  return init(isThreaded);
}

//--------------------------------------------------------------------------------------------
int Loggers::init(bool isThreaded) {
  if (isThreaded)
    m_mtx.reset(new embDB_utilities::DefaultMutex());
  else
    m_mtx.reset(new embDB_utilities::NullMutex());

  if (m_default->init()) EMBDB_THROW("Cannot initialize default logger");

  m_loggers.clear();

  return 0;
}

//--------------------------------------------------------------------------------------------
ILogger& Loggers::getLogger(const std::string& which) {
  ILogger* ret;
  LoggersMap::iterator it;

  m_mtx->lock();

  it = m_loggers.find(which);

  if (it == m_loggers.end()) {
    if (m_default.get() == nullptr) m_default.reset(new LoggerConsole());

    m_loggers[which] = std::unique_ptr<ILogger>(m_default->clone(which));
    ret = m_loggers[which].get();
  } else
    ret = it->second.get();

  m_mtx->unlock();

  return *ret;
}

//--------------------------------------------------------------------------------------------
void Loggers::setMaxLogLevel(LogLevel level) {
  m_mtx->lock();

  for (auto& logger : m_loggers) logger.second->setMaxLogLevel(level);

  m_default->setMaxLogLevel(level);

  m_mtx->unlock();
}

}  // namespace embDB_logging
