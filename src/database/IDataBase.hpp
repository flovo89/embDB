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
#ifndef _EMBDB_I_DATABASE_
#define _EMBDB_I_DATABASE_

#include <list>

#include "../../embdb-layout/cpp/embdb-layout.pb.h"
#include "DbErrorCode.hpp"
#include "db-element/DbElement.hpp"

namespace embDB_database {

class IDataBase {
 public:
  IDataBase();
  virtual ~IDataBase();

  virtual DbErrorCode deserialize() = 0;
  virtual DbErrorCode serialize() = 0;
  virtual DbErrorCode clearAll() = 0;

  virtual DbErrorCode getVersion(uint32_t& version) = 0;
  virtual DbErrorCode getRowCount(uint32_t& count) = 0;

  virtual DbErrorCode createRow(std::string name, DbElementType type,
                                uint32_t maxSize) = 0;
  virtual DbErrorCode rowExists(std::string name, bool& exists) = 0;
  virtual DbErrorCode deleteRow(std::string name) = 0;

  virtual DbErrorCode getAllItems(std::string name,
                                  std::list<DbElement>& elements) = 0;
  virtual DbErrorCode getItemsBetween(std::string name, int64_t start,
                                      int64_t end,
                                      std::list<DbElement>& elements) = 0;
  virtual DbErrorCode addItem(std::string name, const DbElement& element) = 0;
};

}  // namespace embDB_database
#endif
