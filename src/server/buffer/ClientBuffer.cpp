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

#include "ClientBuffer.hpp"

namespace embDB_server {

//--------------------------------------------------------------------------------------------
ClientBuffer::ClientBuffer() : m_readPointer(0), m_writePointer(0) {}

//--------------------------------------------------------------------------------------------
ClientBuffer::~ClientBuffer() {}

//--------------------------------------------------------------------------------------------
bool ClientBuffer::getFromTo(char from, char to, std::string& str) {
  bool startFound = false;
  int tempRead = m_readPointer;
  str = "";
  while (tempRead != m_writePointer) {
    if (m_buffer[tempRead] == from) startFound = true;
    if (startFound) {
      str.push_back(m_buffer[tempRead]);
      if (m_buffer[tempRead] == to) {
        m_readPointer = tempRead;
        return true;
      }
    }
    tempRead++;
    if (tempRead == BUFFERSIZE) tempRead = 0;
  }
  return false;
}

//--------------------------------------------------------------------------------------------
ClientBuffer& ClientBuffer::operator<<(const std::string& str) {
  for (int i = 0; i < (int)str.length(); i++) {
    m_buffer[m_writePointer++] = str[i];
    if (m_writePointer == BUFFERSIZE) m_writePointer = 0;
  }
  return *this;
}

}  // namespace embDB_server
