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

#ifndef _EMBDB_SOCKET_SERVER_UNIX_HPP_
#define _EMBDB_SOCKET_SERVER_UNIX_HPP_

#include <sys/un.h>

#include <string>

#include "../ISocketServer.hpp"
#include "SocketType.hpp"

namespace embDB_socket {

class SocketServer : public ISocketServer {
 public:
  /* Depending of the used constructor, the server acts as TCP socket server or
   * as Unix domain socket server */
  SocketServer(int port);
  SocketServer(const std::string& path);
  virtual ~SocketServer();

  // ISocketServer
  virtual void setAccessMode(int accessMode) override;
  virtual int listen(int backlog) override;
  virtual void close() override;
  virtual int accept(std::unique_ptr<ISocketClient>& client) override;
  virtual embDB_eventloop::IoHandle getIoHandle() override;

 private:
  SocketType m_socketType;
  embDB_eventloop::IoHandle m_ioHandle;
  int m_port;
  std::string m_path;
  int m_accessMode;
};

}  // namespace embDB_socket
#endif  // _COMET_LIB_I_SOCKET_HPP_
