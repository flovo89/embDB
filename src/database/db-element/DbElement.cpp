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
#include "DbElement.hpp"

#include <cstring>

namespace embDB_database {

//--------------------------------------------------------------------------------------------
DbElement::DbElement()
    : m_type(DbElementType::NONE), m_timestamp(c_invalidTs) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(const std::string &data)
    : m_value(static_cast<std::string>(data)),
      m_type(DbElementType::STRING),
      m_timestamp(c_invalidTs) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(const std::string &data, timestamp ts)
    : m_value(static_cast<std::string>(data)),
      m_type(DbElementType::STRING),
      m_timestamp(ts) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(uint32_t data)
    : m_value(static_cast<uint32_t>(data)),
      m_type(DbElementType::UINT32),
      m_timestamp(c_invalidTs) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(uint32_t data, timestamp ts)
    : m_value(static_cast<uint32_t>(data)),
      m_type(DbElementType::UINT32),
      m_timestamp(ts) {}
//--------------------------------------------------------------------------------------------
DbElement::DbElement(int32_t data)
    : m_value(static_cast<int32_t>(data)),
      m_type(DbElementType::INT32),
      m_timestamp(c_invalidTs) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(int32_t data, timestamp ts)
    : m_value(static_cast<int32_t>(data)),
      m_type(DbElementType::INT32),
      m_timestamp(ts) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(uint64_t data)
    : m_value(static_cast<uint64_t>(data)),
      m_type(DbElementType::UINT64),
      m_timestamp(c_invalidTs) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(uint64_t data, timestamp ts)
    : m_value(static_cast<uint64_t>(data)),
      m_type(DbElementType::UINT64),
      m_timestamp(ts) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(int64_t data)
    : m_value(static_cast<int64_t>(data)),
      m_type(DbElementType::INT64),
      m_timestamp(c_invalidTs) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(int64_t data, timestamp ts)
    : m_value(static_cast<int64_t>(data)),
      m_type(DbElementType::INT64),
      m_timestamp(ts) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(float data)
    : m_value(static_cast<float>(data)),
      m_type(DbElementType::FLOAT),
      m_timestamp(c_invalidTs) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(float data, timestamp ts)
    : m_value(static_cast<float>(data)),
      m_type(DbElementType::FLOAT),
      m_timestamp(ts) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(double data)
    : m_value(static_cast<double>(data)),
      m_type(DbElementType::DOUBLE),
      m_timestamp(c_invalidTs) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(double data, timestamp ts)
    : m_value(static_cast<double>(data)),
      m_type(DbElementType::DOUBLE),
      m_timestamp(ts) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(bool data)
    : m_value(static_cast<bool>(data)),
      m_type(DbElementType::BOOL),
      m_timestamp(c_invalidTs) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(bool data, timestamp ts)
    : m_value(static_cast<bool>(data)),
      m_type(DbElementType::BOOL),
      m_timestamp(ts) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(const std::vector<uint8_t> &data)
    : m_value(static_cast<std::vector<uint8_t>>(data)),
      m_type(DbElementType::BYTES),
      m_timestamp(c_invalidTs) {}

//--------------------------------------------------------------------------------------------
DbElement::DbElement(const std::vector<uint8_t> &data, timestamp ts)
    : m_value(static_cast<std::vector<uint8_t>>(data)),
      m_type(DbElementType::BYTES),
      m_timestamp(ts) {}

//--------------------------------------------------------------------------------------------
DbElement::~DbElement() {}

//--------------------------------------------------------------------------------------------
DbElementType DbElement::getType() const {
  if (m_type == DbElementType::NONE) throw "No type";
  return m_type;
}

//--------------------------------------------------------------------------------------------
DbElement::timestamp DbElement::getTimestamp() const {
  if (m_timestamp == c_invalidTs) throw "No timestamp";
  return m_timestamp;
}

//--------------------------------------------------------------------------------------------
std::string DbElement::toString() const {
  if (m_type != DbElementType::STRING) throw "Type mismatch";
  return std::get<std::string>(m_value);
}

//--------------------------------------------------------------------------------------------
uint32_t DbElement::toUint32() const {
  if (m_type != DbElementType::UINT32) throw "Type mismatch";
  return std::get<uint32_t>(m_value);
}

//--------------------------------------------------------------------------------------------
int32_t DbElement::toInt32() const {
  if (m_type != DbElementType::INT32) throw "Type mismatch";
  return std::get<int32_t>(m_value);
}

//--------------------------------------------------------------------------------------------
uint64_t DbElement::toUint64() const {
  if (m_type != DbElementType::UINT64) throw "Type mismatch";
  return std::get<uint64_t>(m_value);
}

//--------------------------------------------------------------------------------------------
int64_t DbElement::toInt64() const {
  if (m_type != DbElementType::INT64) throw "Type mismatch";
  return std::get<int64_t>(m_value);
}

//--------------------------------------------------------------------------------------------
float DbElement::toFloat() const {
  if (m_type != DbElementType::FLOAT) throw "Type mismatch";
  return std::get<float>(m_value);
}

//--------------------------------------------------------------------------------------------
double DbElement::toDouble() const {
  if (m_type != DbElementType::DOUBLE) throw "Type mismatch";
  return std::get<double>(m_value);
}

//--------------------------------------------------------------------------------------------
bool DbElement::toBool() const {
  if (m_type != DbElementType::BOOL) throw "Type mismatch";
  return std::get<bool>(m_value);
}

//--------------------------------------------------------------------------------------------
std::vector<uint8_t> DbElement::toBytes() const {
  if (m_type != DbElementType::BYTES) throw "Type mismatch";
  return std::get<std::vector<uint8_t>>(m_value);
}

//--------------------------------------------------------------------------------------------
DbElement &DbElement::operator=(const std::string &value) {
  m_value = value;
  m_type = DbElementType::STRING;
  return *this;
}

//--------------------------------------------------------------------------------------------
DbElement &DbElement::operator=(const uint32_t &value) {
  m_value = value;
  m_type = DbElementType::UINT32;
  return *this;
}

//--------------------------------------------------------------------------------------------
DbElement &DbElement::operator=(const int32_t &value) {
  m_value = value;
  m_type = DbElementType::INT32;
  return *this;
}

//--------------------------------------------------------------------------------------------
DbElement &DbElement::operator=(const uint64_t &value) {
  m_value = value;
  m_type = DbElementType::UINT64;
  return *this;
}

//--------------------------------------------------------------------------------------------
DbElement &DbElement::operator=(const int64_t &value) {
  m_value = value;
  m_type = DbElementType::INT64;
  return *this;
}

//--------------------------------------------------------------------------------------------
DbElement &DbElement::operator=(const float &value) {
  m_value = value;
  m_type = DbElementType::FLOAT;
  return *this;
}

//--------------------------------------------------------------------------------------------
DbElement &DbElement::operator=(const double &value) {
  m_value = value;
  m_type = DbElementType::DOUBLE;
  return *this;
}

//--------------------------------------------------------------------------------------------
DbElement &DbElement::operator=(const bool &value) {
  m_value = value;
  m_type = DbElementType::BOOL;
  return *this;
}

//--------------------------------------------------------------------------------------------
DbElement &DbElement::operator=(const std::vector<uint8_t> &value) {
  m_value = value;
  m_type = DbElementType::BYTES;
  return *this;
}

//--------------------------------------------------------------------------------------------
bool DbElement::operator==(const DbElement &rhs) const {
  if (m_type != rhs.getType()) return false;

  switch (rhs.getType()) {
    case DbElementType::STRING:
      if (std::get<std::string>(m_value) != rhs.toString()) return false;
      break;
    case DbElementType::UINT32:
      if (std::get<uint32_t>(m_value) != rhs.toUint32()) return false;
      break;
    case DbElementType::INT32:
      if (std::get<int32_t>(m_value) != rhs.toInt32()) return false;
      break;
    case DbElementType::UINT64:
      if (std::get<uint64_t>(m_value) != rhs.toUint64()) return false;
      break;
    case DbElementType::INT64:
      if (std::get<int64_t>(m_value) != rhs.toInt64()) return false;
      break;
    case DbElementType::FLOAT:
      if (std::get<float>(m_value) != rhs.toFloat()) return false;
      break;
    case DbElementType::DOUBLE:
      if (std::get<double>(m_value) != rhs.toDouble()) return false;
      break;
    case DbElementType::BOOL:
      if (std::get<bool>(m_value) != rhs.toBool()) return false;
      break;
    case DbElementType::BYTES:
      if (std::get<std::vector<uint8_t>>(m_value) != rhs.toBytes())
        return false;
      break;
    default:
      break;
  }

  if (m_timestamp != rhs.getTimestamp()) return false;

  return true;
}

//--------------------------------------------------------------------------------------------
bool DbElement::operator!=(const DbElement &rhs) const {
  return !(*this == rhs);
}

}  // namespace embDB_database
