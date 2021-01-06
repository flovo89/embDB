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

#ifndef _EMBDB_SOCKET_CLIENT_POSIX_HPP_
#define _EMBDB_SOCKET_CLIENT_POSIX_HPP_

#include <sys/un.h>

#include <string>

#include "../ISocketClient.hpp"
#include "SocketType.hpp"

namespace embDB_socket {

class SocketClient : public ISocketClient {
 public:
  SocketClient(embDB_eventloop::IoHandle ioHandle);
  virtual ~SocketClient();

  virtual void disconnect() override;
  virtual int read(unsigned char* buffer, unsigned int buffSize) override;
  virtual int write(const unsigned char* buffer,
                    unsigned int buffSize) override;
  virtual embDB_eventloop::IoHandle getIoHandle() override;

 private:
  embDB_eventloop::IoHandle m_ioHandle;
};

}  // namespace embDB_socket
#endif
