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
#ifndef _EMB_DB_DB_ELEMENT_HPP_
#define _EMB_DB_DB_ELEMENT_HPP_

#include <string>
#include <variant>
#include <vector>

#include "../DbElementType.hpp"
#include "DbElement.hpp"

namespace embDB_database {

class DbElement {
 public:
  typedef int64_t timestamp;

  DbElement();
  DbElement(const std::string &data);
  DbElement(const std::string &data, timestamp ts);
  DbElement(uint32_t data);
  DbElement(uint32_t data, timestamp ts);
  DbElement(int32_t data);
  DbElement(int32_t data, timestamp ts);
  DbElement(uint64_t data);
  DbElement(uint64_t data, timestamp ts);
  DbElement(int64_t data);
  DbElement(int64_t data, timestamp ts);
  DbElement(float data);
  DbElement(float data, timestamp ts);
  DbElement(double data);
  DbElement(double data, timestamp ts);
  DbElement(bool data);
  DbElement(bool data, timestamp ts);
  DbElement(const std::vector<uint8_t> &data);
  DbElement(const std::vector<uint8_t> &data, timestamp ts);
  virtual ~DbElement();

  DbElementType getType() const;
  timestamp getTimestamp() const;

  std::string toString() const;
  uint32_t toUint32() const;
  int32_t toInt32() const;
  uint64_t toUint64() const;
  int64_t toInt64() const;
  float toFloat() const;
  double toDouble() const;
  bool toBool() const;
  std::vector<uint8_t> toBytes() const;

  DbElement &operator=(const std::string &value);
  DbElement &operator=(const uint32_t &value);
  DbElement &operator=(const int32_t &value);
  DbElement &operator=(const uint64_t &value);
  DbElement &operator=(const int64_t &value);
  DbElement &operator=(const float &value);
  DbElement &operator=(const double &value);
  DbElement &operator=(const bool &value);
  DbElement &operator=(const std::vector<uint8_t> &value);

  bool operator==(const DbElement &rhs) const;
  bool operator!=(const DbElement &rhs) const;

 private:
  std::variant<std::string, uint32_t, int32_t, uint64_t, int64_t, float, double,
               bool, std::vector<uint8_t>>
      m_value;
  DbElementType m_type;
  timestamp m_timestamp;

  const timestamp c_invalidTs = -1;
};

}  // namespace embDB_database
#endif
