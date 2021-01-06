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

#include "SocketClient.hpp"

#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "../../logging/Loggers.hpp"

namespace embDB_socket {

//--------------------------------------------------------------------------------------------
SocketClient::SocketClient(embDB_eventloop::IoHandle ioHandle)
    : m_ioHandle(ioHandle) {}

//--------------------------------------------------------------------------------------------
SocketClient::~SocketClient() { disconnect(); }

//--------------------------------------------------------------------------------------------
void SocketClient::disconnect() {
  if (m_ioHandle != embDB_eventloop::IHandledIo::INVALID_IO_HANDLE) {
    ::close(m_ioHandle);
    m_ioHandle = embDB_eventloop::IHandledIo::INVALID_IO_HANDLE;
  }
}

//--------------------------------------------------------------------------------------------
int SocketClient::read(unsigned char* buffer, unsigned int buffSize) {
  int readRet = ::read(m_ioHandle, buffer, buffSize);

  if (readRet < 0) return -errno;

  return readRet;
}

//--------------------------------------------------------------------------------------------
int SocketClient::write(const unsigned char* buffer, unsigned int buffSize) {
  int writeRet = ::write(m_ioHandle, buffer, buffSize);

  if (writeRet < 0) return -errno;

  return writeRet;
}

//--------------------------------------------------------------------------------------------
embDB_eventloop::IoHandle SocketClient::getIoHandle() { return m_ioHandle; }

}  // namespace embDB_socket
