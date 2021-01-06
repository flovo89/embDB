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

#ifndef _EMBDB_SERVER_SOCKET_CLIENT_FACTORY_HPP_
#define _EMBDB_SERVER_SOCKET_CLIENT_FACTORY_HPP_

#include "ISocketClientFactory.hpp"

namespace embDB_server {

class SocketClientFactory : public ISocketClientFactory {
 public:
  SocketClientFactory();
  virtual ~SocketClientFactory();

  // ISocketClientFactory
  virtual int buildClient(
      std::unique_ptr<ISocketClient>& newClient,
      embDB_eventloop::IEventLoop& eventLoop, ISocketServer& server,
      std::unique_ptr<embDB_socket::ISocketClient>& socketClient,
      IClientReception& reception) override;
};

}  // namespace embDB_server
#endif
