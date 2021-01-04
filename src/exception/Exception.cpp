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
#include "Exception.hpp"

#include <sstream>

namespace embDB_exception {

//--------------------------------------------------------------------------------------------
Exception::Exception(const std::string& message)
    : m_line(0), m_file(""), m_message(message) {}

//--------------------------------------------------------------------------------------------
Exception::Exception(int line, const std::string& file,
                     const std::string& message)
    : m_line(line), m_file(file), m_message(message) {}

//--------------------------------------------------------------------------------------------
Exception::~Exception() {}

//--------------------------------------------------------------------------------------------
int Exception::getLine() const { return m_line; }

//--------------------------------------------------------------------------------------------
std::string Exception::getFile() const { return m_file; }

//--------------------------------------------------------------------------------------------
std::string Exception::getMessage() const {
  std::stringstream ss;

  if (m_file != "") ss << m_file << " on line " << m_line << ": ";
  ss << m_message;

  return ss.str();
}

//--------------------------------------------------------------------------------------------
const char* Exception::what() const throw() { return "embDB exception"; }

}  // namespace embDB_exception
