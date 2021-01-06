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

#ifndef _EMBDB_I_DAEMONIZER_HPP_
#define _EMBDB_I_DAEMONIZER_HPP_

#include <string>

namespace embDB_process {

class IDaemonizer {
 public:
  IDaemonizer();
  virtual ~IDaemonizer();

  virtual void setPidFile(const std::string& pidFile) = 0;
  virtual int daemonize() = 0;
  virtual void removePidFile() = 0;
};

}  // namespace embDB_process
#endif
