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

#ifndef _EMBDB_DATA_OBJECT_HPP_
#define _EMBDB_DATA_OBJECT_HPP_

#include <list>
#include <string>
#include <vector>

#include "../database/DbElementType.hpp"
#include "../database/db-element/DbElement.hpp"
#include "../errorcode/ErrorCode.hpp"
#include "ProtocolCommand.hpp"

namespace embDB_protocol {

class DataObject {
 public:
  DataObject();
  virtual ~DataObject();

  void setCommand(ProtocolCommand c);
  ProtocolCommand getCommand();
  void setName(std::string name);
  std::string getName();
  void setRowCount(uint32_t count);
  uint32_t getRowCount();
  void setItemsCount(uint32_t count);
  uint32_t getItemsCount();
  void setErrorCode(embDB_errorcode::ErrorCode code);
  embDB_errorcode::ErrorCode getErrorCode();
  void setStartTime(int64_t start);
  int64_t getStartTime();
  void setEndTime(int64_t end);
  int64_t getEndTime();
  void setRowExists(bool exists);
  bool getRowExists();
  void setDbElementType(embDB_database::DbElementType type);
  embDB_database::DbElementType getDbElementType();
  void setDbElement(const embDB_database::DbElement& element);
  embDB_database::DbElement& getDbElement();
  void setDbElements(const std::list<embDB_database::DbElement>& elements);
  std::list<embDB_database::DbElement>& getDbElements();

 private:
  ProtocolCommand m_command;
  std::string m_name;
  uint32_t m_rowCount;
  uint32_t m_itemsCount;
  embDB_errorcode::ErrorCode m_error;
  int64_t m_startTime;
  int64_t m_endTime;
  bool m_rowExists;
  embDB_database::DbElementType m_dbElementType;
  embDB_database::DbElement m_element;
  std::list<embDB_database::DbElement> m_elements;
};

}  // namespace embDB_protocol
#endif
