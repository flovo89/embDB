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

#include "DataObject.hpp"

namespace embDB_protocol {

//--------------------------------------------------------------------------------------------
DataObject::DataObject()
    : m_command(ProtocolCommand::NONE),
      m_name(""),
      m_rowCount(0),
      m_itemsCount(0),
      m_error(embDB_errorcode::ERROR_SUCCESS),
      m_startTime(0),
      m_endTime(0),
      m_rowExists(false),
      m_dbElementType(embDB_database::DbElementType::NONE) {}

//--------------------------------------------------------------------------------------------
DataObject::~DataObject() {}

//--------------------------------------------------------------------------------------------
void DataObject::setCommand(ProtocolCommand c) { m_command = c; }

//--------------------------------------------------------------------------------------------
ProtocolCommand DataObject::getCommand() { return m_command; }

//--------------------------------------------------------------------------------------------
void DataObject::setName(std::string name) { m_name = name; }

//--------------------------------------------------------------------------------------------
std::string DataObject::getName() { return m_name; }

//--------------------------------------------------------------------------------------------
void DataObject::setRowCount(uint32_t count) { m_rowCount = count; }

//--------------------------------------------------------------------------------------------
uint32_t DataObject::getRowCount() { return m_rowCount; }

//--------------------------------------------------------------------------------------------
void DataObject::setItemsCount(uint32_t count) { m_itemsCount = count; }

//--------------------------------------------------------------------------------------------
uint32_t DataObject::getItemsCount() { return m_itemsCount; }

//--------------------------------------------------------------------------------------------
void DataObject::setErrorCode(embDB_errorcode::ErrorCode code) {
  m_error = code;
}

//--------------------------------------------------------------------------------------------
embDB_errorcode::ErrorCode DataObject::getErrorCode() { return m_error; }

//--------------------------------------------------------------------------------------------
void DataObject::setStartTime(int64_t start) { m_startTime = start; }

//--------------------------------------------------------------------------------------------
int64_t DataObject::getStartTime() { return m_startTime; }

//--------------------------------------------------------------------------------------------
void DataObject::setEndTime(int64_t end) { m_endTime = end; }

//--------------------------------------------------------------------------------------------
int64_t DataObject::getEndTime() { return m_endTime; }

//--------------------------------------------------------------------------------------------
void DataObject::setRowExists(bool exists) { m_rowExists = exists; }

//--------------------------------------------------------------------------------------------
bool DataObject::getRowExists() { return m_rowExists; }

//--------------------------------------------------------------------------------------------
void DataObject::setDbElementType(embDB_database::DbElementType type) {
  m_dbElementType = type;
}

//--------------------------------------------------------------------------------------------
embDB_database::DbElementType DataObject::getDbElementType() {
  return m_dbElementType;
}

//--------------------------------------------------------------------------------------------
void DataObject::setDbElement(const embDB_database::DbElement& element) {
  m_element = element;
}

//--------------------------------------------------------------------------------------------
embDB_database::DbElement& DataObject::getDbElement() { return m_element; }

}  // namespace embDB_protocol