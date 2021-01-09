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

#ifndef _EMBDB_CLIENT_BUFFER_HPP_
#define _EMBDB_CLIENT_BUFFER_HPP_

#include <string>

namespace embDB_server {

#define BUFFERSIZE 8192

class ClientBuffer {
 public:
  ClientBuffer();
  virtual ~ClientBuffer();

  bool popFromTo(char from, char to, std::string& str);
  bool peekFromTo(char from, char to, std::string& str);

  ClientBuffer& operator<<(const std::string& str);

 private:
  char m_buffer[BUFFERSIZE];
  int m_readPointer;
  int m_writePointer;

  bool fromTo(bool alter, char from, char to, std::string& str);
};
}  // namespace embDB_server
#endif