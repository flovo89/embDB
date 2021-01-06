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

#ifndef _EMBDB_I_SOCKET_SERVER_HPP_
#define _EMBDB_I_SOCKET_SERVER_HPP_

#include <memory>

#include "ISocketClient.hpp"

namespace embDB_socket {

class ISocketServer {
 public:
  ISocketServer();
  virtual ~ISocketServer();

  virtual void setAccessMode(int accessMode) = 0;
  virtual int listen(int backlog) = 0;
  virtual void close() = 0;
  virtual int accept(std::unique_ptr<ISocketClient>& client) = 0;
  virtual embDB_eventloop::IoHandle getIoHandle() = 0;
};

}  // namespace embDB_socket
#endif
