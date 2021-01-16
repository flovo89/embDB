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
#ifndef _EMBDB_DB_GUARD_
#define _EMBDB_DB_GUARD_

#include "../../utilities/IMutex.hpp"
#include "../IDataBaseCircular.hpp"
#include "../db-layout/DbLayoutCircular.hpp"
#include "../db-layout/DbLayoutLinear.hpp"

namespace embDB_database {

class DbGuard : public IDataBaseCircular, public IDataBaseLinear {
 public:
  DbGuard(std::unique_ptr<IDataBaseCircular> circular,
          std::unique_ptr<IDataBaseLinear> linear,
          std::unique_ptr<embDB_utilities::IMutex> mutex);
  virtual ~DbGuard();

  // IDataBase
  virtual int init() override;
  virtual int deinit() override;
  virtual DbErrorCode serialize() override;
  virtual DbErrorCode clearAll() override;

  // IDataBaseCircular
  virtual DbErrorCode getVersionCircular(uint32_t& version) override;

  virtual DbErrorCode getRowCount(uint32_t& count) override;

  virtual DbErrorCode createRow(std::string name, DbElementType type,
                                uint32_t maxSize) override;
  virtual DbErrorCode rowExists(std::string name, bool& exists) override;
  virtual DbErrorCode deleteRow(std::string name) override;

  virtual DbErrorCode getAllItemsCircular(
      std::string name, std::list<DbElement>& elements) override;
  virtual DbErrorCode getItemsBetweenCircular(
      std::string name, int64_t start, int64_t end,
      std::list<DbElement>& elements) override;
  virtual DbErrorCode addItemCircular(std::string name,
                                      const DbElement& element) override;

  // IDataBaseLinear
  virtual DbErrorCode getVersionLinear(uint32_t& version) override;
  virtual DbErrorCode getAllItemsLinear(
      std::string name, std::list<DbElement>& elements) override;
  virtual DbErrorCode getItemsBetweenLinear(
      std::string name, int64_t start, int64_t end,
      std::list<DbElement>& elements) override;
  virtual DbErrorCode addItemLinear(std::string name,
                                    const DbElement& element) override;

 private:
  std::unique_ptr<IDataBaseCircular> m_circular;
  std::unique_ptr<IDataBaseLinear> m_linear;
  std::unique_ptr<embDB_utilities::IMutex> m_mutex;
};

}  // namespace embDB_database
#endif
