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

#ifndef _EMBDB_LOGGER_FACTORY_HPP_
#define _EMBDB_LOGGER_FACTORY_HPP_

#include <memory>

#include "ILogger.hpp"

namespace embDB_logging {

class LoggerFactory {
 public:
  LoggerFactory() = delete;
  ~LoggerFactory() = delete;
  LoggerFactory(LoggerFactory const&) = delete;
  LoggerFactory(LoggerFactory&&) = delete;
  LoggerFactory& operator=(LoggerFactory const&) = delete;
  LoggerFactory& operator=(LoggerFactory&&) = delete;

  static int createDefaultLogger(std::unique_ptr<ILogger>& logger,
                                 const std::string& config);
};

}  // namespace embDB_logging
#endif
