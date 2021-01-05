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
#include "../IDataBase.hpp"
#include "../db-layout/DbLayout.hpp"

namespace embDB_database {

class DbGuard : public IDataBase {
 public:
  DbGuard(std::unique_ptr<IDataBase> layout,
          std::unique_ptr<embDB_utilities::IMutex> mutex);
  virtual ~DbGuard();

  // IDataBase
  virtual DbErrorCode deserialize() override;
  virtual DbErrorCode serialize() override;
  virtual DbErrorCode clearAll() override;

  virtual DbErrorCode getVersion(uint32_t& version) override;
  virtual DbErrorCode getRowCount(uint32_t& count) override;

  virtual DbErrorCode createRow(std::string name, DbElementType type,
                                uint32_t maxSize) override;
  virtual DbErrorCode rowExists(std::string name, bool& exists) override;
  virtual DbErrorCode deleteRow(std::string name) override;

  virtual DbErrorCode getAllItems(std::string name,
                                  std::list<DbElement>& elements) override;
  virtual DbErrorCode getItemsBetween(std::string name, int64_t start,
                                      int64_t end,
                                      std::list<DbElement>& elements) override;
  virtual DbErrorCode addItem(std::string name,
                              const DbElement& element) override;

 private:
  std::unique_ptr<IDataBase> m_layout;
  std::unique_ptr<embDB_utilities::IMutex> m_mutex;
};

}  // namespace embDB_database
#endif
