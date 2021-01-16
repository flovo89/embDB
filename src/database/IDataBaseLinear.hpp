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
#ifndef _EMBDB_I_DATABASE_LINEAR_HPP_
#define _EMBDB_I_DATABASE_LINEAR_HPP_

#include "IDataBase.hpp"

namespace embDB_database {

class IDataBaseLinear : public IDataBase {
 public:
  IDataBaseLinear();
  virtual ~IDataBaseLinear();

  virtual DbErrorCode getVersionLinear(uint32_t& version) = 0;

  virtual DbErrorCode getAllItemsLinear(std::string name,
                                        std::list<DbElement>& elements) = 0;
  virtual DbErrorCode getItemsBetweenLinear(std::string name, int64_t start,
                                            int64_t end,
                                            std::list<DbElement>& elements) = 0;
  virtual DbErrorCode addItemLinear(std::string name,
                                    const DbElement& element) = 0;
};

}  // namespace embDB_database
#endif
