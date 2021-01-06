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

#include "SocketClient.hpp"

#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "../../logging/Loggers.hpp"

namespace embDB_server {

//--------------------------------------------------------------------------------------------
SocketClient::SocketClient(
    embDB_eventloop::IEventLoop& eventLoop, ISocketServer& socketServer,
    std::unique_ptr<embDB_socket::ISocketClient>& socketClient,
    IClientReception& reception, std::unique_ptr<ClientBuffer>& buffer)
    : m_eventLoop(eventLoop),
      m_socketServer(socketServer),
      m_socketClient(std::move(socketClient)),
      m_clientReception(reception),
      m_buffer(std::move(buffer)) {}

//--------------------------------------------------------------------------------------------
SocketClient::~SocketClient() {}

//--------------------------------------------------------------------------------------------
int SocketClient::init() {
  if (m_eventLoop.registerHandledIo(*this,
                                    embDB_eventloop::IEventLoop::READ |
                                        embDB_eventloop::IEventLoop::PERSIST))
    return -1;

  return 0;
}

//--------------------------------------------------------------------------------------------
void SocketClient::disconnect() {
  m_eventLoop.deregisterHandledIo(*this);
  m_socketClient->disconnect();
}

//--------------------------------------------------------------------------------------------
void SocketClient::writeString(std::string& data) {
  int totalWritten = 0;
  int writeRet;
  int dataSize = (int)data.size();

  while (totalWritten < dataSize) {
    writeRet = write(getIoHandle(), (void*)(data.c_str() + totalWritten),
                     dataSize - totalWritten);

    if (writeRet < 0) {
      LOG_ER() << "Cannot write data string: " << std::strerror(errno);
      return;
    }

    totalWritten += writeRet;
  }
}

//--------------------------------------------------------------------------------------------
ClientBuffer& SocketClient::getClientBuffer() { return *m_buffer; }

//--------------------------------------------------------------------------------------------
embDB_eventloop::IoHandle SocketClient::getIoHandle() {
  return m_socketClient->getIoHandle();
}

//--------------------------------------------------------------------------------------------
void SocketClient::readReady() {
  unsigned int buffSize = 128;
  unsigned char buffer[buffSize];
  int readSize;

  readSize = m_socketClient->read(buffer, buffSize);
  if (readSize <= 0) {
    if (readSize < 0)
      LOG_ER() << "Cannot read from socket: " << strerror(-readSize);
    else
      LOG_IN() << "Remote socket closed";

    m_socketServer.onClientDisconnected(m_socketClient->getIoHandle());
  } else {
    *m_buffer << std::string((char*)buffer, readSize);
    m_clientReception.notifier(*this);
  }
}

}  // namespace embDB_server
