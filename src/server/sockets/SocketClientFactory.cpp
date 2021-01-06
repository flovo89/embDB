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

#include "SocketClientFactory.hpp"

#include "SocketClient.hpp"

namespace embDB_server {

//--------------------------------------------------------------------------------------------
SocketClientFactory::SocketClientFactory() {}

//--------------------------------------------------------------------------------------------
SocketClientFactory::~SocketClientFactory() {}

//--------------------------------------------------------------------------------------------
int SocketClientFactory::buildClient(
    std::unique_ptr<ISocketClient>& newClient,
    embDB_eventloop::IEventLoop& eventLoop, ISocketServer& server,
    std::unique_ptr<embDB_socket::ISocketClient>& socketClient,
    IClientReception& reception) {
  std::unique_ptr<ClientBuffer> buf(new ClientBuffer());
  newClient.reset(
      new SocketClient(eventLoop, server, socketClient, reception, buf));

  return 0;
}

}  // namespace embDB_server
