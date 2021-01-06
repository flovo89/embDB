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

#ifndef _EMBDB_SERVER_SOCKET_SERVER_HPP_
#define _EMBDB_SERVER_SOCKET_SERVER_HPP_

#include <string>
#include <unordered_map>

#include "../../eventloop/IEventLoop.hpp"
#include "../../socket/ISocketServer.hpp"
#include "../ISocketServer.hpp"
#include "ISocketClientFactory.hpp"

namespace embDB_server {

class SocketServer : public ISocketServer, public embDB_eventloop::IHandledIo {
 public:
  static const int BACKLOG_COUNT;

  SocketServer(embDB_eventloop::IEventLoop& el,
               std::unique_ptr<embDB_socket::ISocketServer> socketServer,
               std::unique_ptr<ISocketClientFactory> clientFactory,
               IClientReception& receiver);
  ~SocketServer();

  // ISocketServer
  virtual int init() override;
  virtual void close() override;
  virtual void onClientDisconnected(embDB_eventloop::IoHandle id) override;

  // embDB_eventloop::IHandledIo
  virtual embDB_eventloop::IoHandle getIoHandle();
  virtual void readReady();

 private:
  embDB_eventloop::IEventLoop& m_el;
  std::unique_ptr<embDB_socket::ISocketServer> m_socketServer;
  std::unique_ptr<ISocketClientFactory> m_clientFactory;
  IClientReception& m_clientReception;

 protected:
  typedef std::unordered_map<embDB_eventloop::IoHandle,
                             std::unique_ptr<ISocketClient>>
      SocketClientsList;
  SocketClientsList m_clients;
};

}  // namespace embDB_server

#endif
