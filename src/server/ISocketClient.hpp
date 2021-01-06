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

#ifndef _EMBDB_SERVER_I_SOCKET_CLIENT_HPP_
#define _EMBDB_SERVER_I_SOCKET_CLIENT_HPP_

#include <string>

#include "buffer/ClientBuffer.hpp"

namespace embDB_server {

class ISocketClient {
 public:
  ISocketClient();
  virtual ~ISocketClient();

  virtual int init() = 0;
  virtual void disconnect() = 0;

  virtual void writeString(std::string& data) = 0;
  virtual ClientBuffer& getClientBuffer() = 0;
};

}  // namespace embDB_server
#endif