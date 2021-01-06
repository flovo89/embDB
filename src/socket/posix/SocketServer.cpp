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

#include "SocketServer.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../exception/Exception.hpp"
#include "../../logging/Loggers.hpp"
#include "SocketClient.hpp"

namespace embDB_socket {

//--------------------------------------------------------------------------------------------
// Constructor for socket socket server
SocketServer::SocketServer(int port)
    : m_socketType(TCP_SOCKET),
      m_ioHandle(embDB_eventloop::IHandledIo::INVALID_IO_HANDLE),
      m_port(port),
      m_path(""),
      m_accessMode(-1) {}

//--------------------------------------------------------------------------------------------
// Constructor for Unix domain socket server
SocketServer::SocketServer(const std::string& path)
    : m_socketType(UNIX_SOCKET),
      m_ioHandle(embDB_eventloop::IHandledIo::INVALID_IO_HANDLE),
      m_port(-1),
      m_path(path),
      m_accessMode(-1) {}

//--------------------------------------------------------------------------------------------
SocketServer::~SocketServer() {}

//--------------------------------------------------------------------------------------------
void SocketServer::setAccessMode(int accessMode) { m_accessMode = accessMode; }

//--------------------------------------------------------------------------------------------
int SocketServer::listen(int backlog) {
  int sockadrLen;
  int opt = 1;
  struct sockaddr_un sockAddrUn;
  struct sockaddr_in sockAddrIn;

  // Close current socket if opened
  close();

  if (m_socketType == UNIX_SOCKET) {
    // Create the socket
    m_ioHandle = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_ioHandle < 0) {
      LOG_ER() << "Cannot create socket: " << ::strerror(errno);
      close();
      return -1;
    }

    // Bind
    sockAddrUn.sun_family = AF_UNIX;
    ::strcpy(sockAddrUn.sun_path, m_path.c_str());
    sockadrLen = strlen(sockAddrUn.sun_path) + sizeof(sockAddrUn.sun_family);
    if (bind(m_ioHandle, (struct sockaddr*)&sockAddrUn, sockadrLen) == -1) {
      LOG_ER() << "Cannot bind UNIX domain socket: " << ::strerror(errno);
      return -1;
    }

    // Set mode when requested
    if (m_accessMode >= 0) {
      if (chmod(m_path.c_str(), m_accessMode))
        LOG_WA() << "Cannot change unix socket mode: " << ::strerror(errno);
    }
  } else if (m_socketType == TCP_SOCKET) {
    // Create the tcp socket
    m_ioHandle = socket(AF_INET, SOCK_STREAM, 0);
    if (m_ioHandle < 0) {
      LOG_ER() << "Cannot create tcp socket: " << ::strerror(errno);
      close();
      return -1;
    }

    // Forcefully attach socket to port
    if (setsockopt(m_ioHandle, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
      LOG_ER() << "Failed to set tcp socket option: " << ::strerror(errno);
      close();
      return -1;
    }

    // Bind
    sockAddrIn.sin_family = AF_INET;
    sockAddrIn.sin_addr.s_addr = INADDR_ANY;
    sockAddrIn.sin_port = htons(m_port);
    if (bind(m_ioHandle, (struct sockaddr*)&sockAddrIn, sizeof(sockAddrIn)) ==
        -1) {
      LOG_ER() << "Cannot bind tcp socket: " << ::strerror(errno);
      return -1;
    }

  } else {
    EMBDB_THROW("Invalid sockettype");
  }

  // Listen
  if (::listen(m_ioHandle, backlog)) {
    LOG_ER() << "Cannot listen to socket: " << ::strerror(errno);
    return -1;
  }

  return 0;
}

//--------------------------------------------------------------------------------------------
void SocketServer::close() {
  if (m_ioHandle != embDB_eventloop::IHandledIo::INVALID_IO_HANDLE) {
    ::close(m_ioHandle);
    if (m_socketType == UNIX_SOCKET) ::unlink(m_path.c_str());
    m_ioHandle = embDB_eventloop::IHandledIo::INVALID_IO_HANDLE;
  }
}

//--------------------------------------------------------------------------------------------
int SocketServer::accept(std::unique_ptr<ISocketClient>& client) {
  struct sockaddr_un remoteAddrUn;
  struct sockaddr_in remoteAddrIn;
  socklen_t remotAddSize = (m_socketType == UNIX_SOCKET) ? sizeof(remoteAddrUn)
                                                         : sizeof(remoteAddrIn);
  int newFd;

  newFd = ::accept(
      m_ioHandle,
      ((m_socketType == UNIX_SOCKET) ? (struct sockaddr*)&remoteAddrUn
                                     : (struct sockaddr*)&remoteAddrIn),
      &remotAddSize);
  if (newFd < 0) {
    LOG_ER() << "Socket accept error: " << strerror(errno);
    return -1;
  }

  client.reset(new SocketClient(newFd));
  return 0;
}

//--------------------------------------------------------------------------------------------
embDB_eventloop::IoHandle SocketServer::getIoHandle() { return m_ioHandle; }

}  // namespace embDB_socket
