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

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "../../logging/Loggers.hpp"

namespace embDB_server {

const int SocketServer::BACKLOG_COUNT = 5;

//--------------------------------------------------------------------------------------------
SocketServer::SocketServer(
    embDB_eventloop::IEventLoop& el,
    std::unique_ptr<embDB_socket::ISocketServer> socketServer,
    std::unique_ptr<ISocketClientFactory> clientFactory,
    IClientReception& receiver)
    : m_el(el),
      m_socketServer(std::move(socketServer)),
      m_clientFactory(std::move(clientFactory)),
      m_clientReception(receiver) {}

//--------------------------------------------------------------------------------------------
SocketServer::~SocketServer() {}

//--------------------------------------------------------------------------------------------
int SocketServer::init() {
  if (m_socketServer->listen(BACKLOG_COUNT)) return -1;
  if (m_el.registerHandledIo(*this, embDB_eventloop::IEventLoop::READ |
                                        embDB_eventloop::IEventLoop::PERSIST))
    return -1;

  return 0;
}

//--------------------------------------------------------------------------------------------
void SocketServer::close() {
  for (auto& clientIt : m_clients) clientIt.second->disconnect();
  m_clients.clear();
  m_socketServer->close();
}

//--------------------------------------------------------------------------------------------
void SocketServer::onClientDisconnected(embDB_eventloop::IoHandle id) {
  auto client = m_clients.find(id);

  // Should not happen
  if (client == m_clients.end()) {
    LOG_WA() << "BUG? Client " << id << " not found when disconnecting!";
    return;
  }

  client->second->disconnect();
  m_clients.erase(client);

  LOG_DB() << "Client " << id << " disconnected";
}

//--------------------------------------------------------------------------------------------
embDB_eventloop::IoHandle SocketServer::getIoHandle() {
  return m_socketServer->getIoHandle();
}

//--------------------------------------------------------------------------------------------
void SocketServer::readReady() {
  std::unique_ptr<embDB_socket::ISocketClient> newSocket;
  std::unique_ptr<ISocketClient> newClient;
  embDB_eventloop::IoHandle clientHandle;

  LOG_DB() << "New incoming connection";

  if (m_socketServer->accept(newSocket)) return;
  clientHandle = newSocket->getIoHandle();

  if (m_clientFactory->buildClient(newClient, m_el, *this, newSocket,
                                   m_clientReception)) {
    newSocket->disconnect();
    return;
  }

  if (newClient->init()) {
    newSocket->disconnect();
    return;
  }

  m_clients[clientHandle] = std::move(newClient);

  LOG_DB() << "Client " << clientHandle << " connected!";
}

}  // namespace embDB_server
