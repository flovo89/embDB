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
#ifndef _EMBDB_DB_LAYOUT_CIRCULAR_HPP_
#define _EMBDB_DB_LAYOUT_CIRCULAR_HPP_

#include "../../file-io/FileReader.hpp"
#include "../../file-io/FileWriter.hpp"
#include "../../utilities/IHasher.hpp"
#include "../../utilities/ITimestamper.hpp"
#include "../IDataBaseCircular.hpp"

namespace embDB_database {

using namespace google::protobuf;
using namespace embDB_protolayout;

class DbLayoutCircular : public IDataBaseCircular {
 public:
  DbLayoutCircular(std::unique_ptr<embDB_fileio::FileReader> reader,
                   std::unique_ptr<embDB_fileio::FileWriter> writer,
                   std::unique_ptr<embDB_utilities::IHasher> hasher,
                   std::unique_ptr<embDB_utilities::ITimestamper> timestamper);
  virtual ~DbLayoutCircular();

  friend class DbGuard;

 private:
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

  std::unique_ptr<embDB_fileio::FileReader> m_reader;
  std::unique_ptr<embDB_fileio::FileWriter> m_writer;
  std::unique_ptr<embDB_utilities::IHasher> m_hasher;
  std::unique_ptr<embDB_utilities::ITimestamper> m_timestamper;

  bool m_isDeserialized;
  embDB_protolayout::DataLayoutCircular m_dataLayout;

  const uint32_t c_version = 1;

  int getRow(std::string name, DataRowCircular& row) const;
  int getRowMutable(std::string name, DataRowCircular** row);
  void getName(const DataRowCircular& row, std::string& name) const;
  void getType(const DataRowCircular& row, DbElementType& type) const;
  void getOverflow(const DataRowCircular& row, bool& overflow) const;
  void setOverflow(DataRowCircular* row, bool overflow);
  void getMaxItems(const DataRowCircular& row, uint32_t& maxItems) const;
  void getCurItem(const DataRowCircular& row, uint32_t& curItem) const;
  void setCurItem(DataRowCircular* row, uint32_t curItem);
  void getDataItems(const DataRowCircular& row,
                    RepeatedPtrField<DataItem>& items) const;
  void getDataItemsMutable(DataRowCircular* row, RepeatedPtrField<DataItem>** items);
  void getDataElement(DbElementType type, const DataItem* item,
                      DbElement& element);
  void setDataItem(const DbElement& element, DataItem* item);
};

}  // namespace embDB_database
#endif
