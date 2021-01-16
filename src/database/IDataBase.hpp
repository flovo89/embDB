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
#ifndef _EMBDB_I_DATABASE_HPP_
#define _EMBDB_I_DATABASE_HPP_

#include <list>
#include <string>

#include "DbErrorCode.hpp"
#include "db-element/DbElement.hpp"

namespace embDB_database {

class IDataBase {
 public:
  IDataBase();
  virtual ~IDataBase();

  virtual int init() = 0;
  virtual int deinit() = 0;
  virtual DbErrorCode serialize() = 0;
  virtual DbErrorCode clearAll() = 0;
};

}  // namespace embDB_database
#endif
