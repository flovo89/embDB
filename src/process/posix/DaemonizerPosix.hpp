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

#ifndef _EMBDB_DAEMONIZER_POSIX_HPP_
#define _EMBDB_DAEMONIZER_POSIX_HPP_

#include "../IDaemonizer.hpp"

namespace embDB_process {

class DaemonizerPosix : public IDaemonizer {
 public:
  DaemonizerPosix();
  ~DaemonizerPosix();

  void setPidFile(const std::string& pidFile) override;
  int daemonize() override;
  void removePidFile() override;

 private:
  std::string m_pidFile;

  void writePidFile();
};

}  // namespace embDB_process
#endif
