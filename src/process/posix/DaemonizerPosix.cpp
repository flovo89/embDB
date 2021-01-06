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

#include "DaemonizerPosix.hpp"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <sstream>

#include "../../logging/Loggers.hpp"

namespace embDB_process {

//--------------------------------------------------------------------------------------------
DaemonizerPosix::DaemonizerPosix() {}

//--------------------------------------------------------------------------------------------
DaemonizerPosix::~DaemonizerPosix() {}

//--------------------------------------------------------------------------------------------
void DaemonizerPosix::setPidFile(const std::string& pidFile) {
  m_pidFile = pidFile;
}

//--------------------------------------------------------------------------------------------
int DaemonizerPosix::daemonize() {
  pid_t pid;

  pid = fork();

  if (pid == -1) {
    // Fork error
    LOG_ER() << "Forking error: " << strerror(errno);
    return -1;
  } else if (pid == 0) {
    // Child process
    fclose(stdout);
    fclose(stderr);
    if (setsid() < 0) {
      LOG_ER() << "Cannot create a new process group: " << strerror(errno);
      return -1;
    }
    writePidFile();
    return 0;
  } else {
    // parent process
    return pid;
  }
}

//--------------------------------------------------------------------------------------------
void DaemonizerPosix::removePidFile() {
  if (m_pidFile == "") return;

  if (remove(m_pidFile.c_str()) != 0)
    LOG_ER() << "Cannot remove PID file " << m_pidFile << ": "
             << strerror(errno);

  LOG_DB() << "PID file " << m_pidFile << " removed";
}

//--------------------------------------------------------------------------------------------
void DaemonizerPosix::writePidFile() {
  int pidFd;
  std::stringstream pidValue;

  if (m_pidFile == "") {
    LOG_WA() << "PID file not set while daemonizing, not creating";
    return;
  }

  pidFd = open(m_pidFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
               S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  if (pidFd < 0) {
    LOG_ER() << "Error opening PID file " << m_pidFile << ": "
             << strerror(errno);
    return;
  }

  pidValue << getpid() << std::endl;
  if (write(pidFd, pidValue.str().c_str(), pidValue.str().size()) < 0)
    LOG_ER() << "Cannot write PID file " << m_pidFile << ": "
             << strerror(errno);

  LOG_DB() << "PID file " << m_pidFile << " created";

  close(pidFd);
}

}  // namespace embDB_process
