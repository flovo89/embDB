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
#ifndef _EMBDB_I_DATABASE_CIRCULAR_HPP_
#define _EMBDB_I_DATABASE_CIRCULAR_HPP_

#include <list>

#include "DbErrorCode.hpp"
#include "IDataBase.hpp"
#include "db-element/DbElement.hpp"

namespace embDB_database {

class IDataBaseCircular : public IDataBase {
 public:
  IDataBaseCircular();
  virtual ~IDataBaseCircular();

  virtual DbErrorCode getVersionCircular(uint32_t& version) = 0;

  virtual DbErrorCode getRowCount(uint32_t& count) = 0;

  virtual DbErrorCode createRow(std::string name, DbElementType type,
                                uint32_t maxSize) = 0;
  virtual DbErrorCode rowExists(std::string name, bool& exists) = 0;
  virtual DbErrorCode deleteRow(std::string name) = 0;

  virtual DbErrorCode getAllItemsCircular(std::string name,
                                          std::list<DbElement>& elements) = 0;
  virtual DbErrorCode getItemsBetweenCircular(
      std::string name, int64_t start, int64_t end,
      std::list<DbElement>& elements) = 0;
  virtual DbErrorCode addItemCircular(std::string name,
                                      const DbElement& element) = 0;
};

}  // namespace embDB_database
#endif
