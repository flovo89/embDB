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
#ifndef _EMBDB_EXCEPTION_HPP_
#define _EMBDB_EXCEPTION_HPP_

#include <exception>
#include <string>

namespace embDB_exception {

class Exception : public std::exception {
 public:
  Exception(const std::string& message);
  Exception(int line, const std::string& file, const std::string& message);
  virtual ~Exception();

  int getLine() const;
  std::string getFile() const;
  std::string getMessage() const;

  // std::exception
  virtual const char* what() const throw() override;

 private:
  int m_line;
  std::string m_file;
  std::string m_message;
};

#define EMBDB_THROW(msg) \
  throw embDB_exception::Exception(__LINE__, __FILE__, msg);

}  // namespace embDB_exception
#endif
