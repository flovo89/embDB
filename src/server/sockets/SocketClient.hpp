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

#ifndef _EMBDB_SERVER_SOCKET_CLIENT_HPP_
#define _EMBDB_SERVER_SOCKET_CLIENT_HPP_

#include <memory>
#include <string>

#include "../../eventloop/IEventLoop.hpp"
#include "../../socket/ISocketClient.hpp"
#include "../IClientReception.hpp"
#include "../ISocketClient.hpp"
#include "../ISocketServer.hpp"

namespace embDB_server {

class SocketClient : public ISocketClient, public embDB_eventloop::IHandledIo {
 public:
  SocketClient(embDB_eventloop::IEventLoop& eventLoop, ISocketServer& socketServer,
               std::unique_ptr<embDB_socket::ISocketClient>& socketClient,
               IClientReception& reception,
               std::unique_ptr<ClientBuffer>& buffer);
  ~SocketClient();

  // ISocketClient
  virtual int init() override;
  virtual void disconnect() override;
  virtual void writeString(std::string& data) override;
  virtual ClientBuffer& getClientBuffer() override;

  // embDB_eventloop::IHandledIo
  virtual embDB_eventloop::IoHandle getIoHandle() override;
  virtual void readReady() override;

 private:
  embDB_eventloop::IEventLoop& m_eventLoop;
  ISocketServer& m_socketServer;
  std::unique_ptr<embDB_socket::ISocketClient> m_socketClient;
  IClientReception& m_clientReception;
  std::unique_ptr<ClientBuffer> m_buffer;
};

}  // namespace embDB_server

#endif
