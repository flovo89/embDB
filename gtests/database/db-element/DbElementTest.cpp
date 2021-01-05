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

#include <iostream>
#include <sstream>

#include "../../../src/database/db-element/DbElement.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
namespace embDB_database {
namespace gtests {

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, emptyElement) {
  DbElement el;
  EXPECT_ANY_THROW(el.getTimestamp());
  EXPECT_ANY_THROW(el.getType());
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, stringElement) {
  std::string teststr = "whateveryouwant";
  DbElement el(teststr);

  EXPECT_EQ(el.getType(), DbElementType::STRING);
  EXPECT_ANY_THROW(el.getTimestamp());
  EXPECT_FALSE(el.hasTimestamp());

  EXPECT_EQ(el.toString(), teststr);
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(teststr);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(teststr, 1234);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4321);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, stringElementTs) {
  std::string teststr = "whateveryouwant";
  DbElement::timestamp ts = 1234567890;
  DbElement el(teststr, ts);

  EXPECT_EQ(el.getType(), DbElementType::STRING);
  EXPECT_EQ(el.getTimestamp(), ts);
  EXPECT_TRUE(el.hasTimestamp());

  EXPECT_EQ(el.toString(), teststr);
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(teststr, ts);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(teststr, ts - 1);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4321);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, uint32Element) {
  uint32_t testuint32 = 12345678;
  DbElement el(testuint32);

  EXPECT_EQ(el.getType(), DbElementType::UINT32);
  EXPECT_ANY_THROW(el.getTimestamp());
  EXPECT_FALSE(el.hasTimestamp());

  EXPECT_EQ(el.toUint32(), testuint32);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testuint32);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testuint32, 1234);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4321);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, uint32ElementTs) {
  uint32_t testuint32 = 12345678;
  DbElement::timestamp ts = 1234567890;
  DbElement el(testuint32, ts);

  EXPECT_EQ(el.getType(), DbElementType::UINT32);
  EXPECT_EQ(el.getTimestamp(), ts);
  EXPECT_TRUE(el.hasTimestamp());

  EXPECT_EQ(el.toUint32(), testuint32);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testuint32, ts);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testuint32, ts - 1);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4321);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, int32Element) {
  int32_t testint32 = -12345678;
  DbElement el(testint32);

  EXPECT_EQ(el.getType(), DbElementType::INT32);
  EXPECT_ANY_THROW(el.getTimestamp());
  EXPECT_FALSE(el.hasTimestamp());

  EXPECT_EQ(el.toInt32(), testint32);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testint32);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testint32, 1234);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4321);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, int32ElementTs) {
  int32_t testint32 = -12345678;
  DbElement::timestamp ts = 1234567890;
  DbElement el(testint32, ts);

  EXPECT_EQ(el.getType(), DbElementType::INT32);
  EXPECT_EQ(el.getTimestamp(), ts);
  EXPECT_TRUE(el.hasTimestamp());

  EXPECT_EQ(el.toInt32(), testint32);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testint32, ts);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testint32, ts - 1);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4321);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, uint64Element) {
  uint64_t testuint64 = 1234567890;
  DbElement el(testuint64);

  EXPECT_EQ(el.getType(), DbElementType::UINT64);
  EXPECT_ANY_THROW(el.getTimestamp());
  EXPECT_FALSE(el.hasTimestamp());

  EXPECT_EQ(el.toUint64(), testuint64);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testuint64);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testuint64, 1234);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, uint64ElementTs) {
  uint64_t testuint64 = 1234567890;
  DbElement::timestamp ts = 1234567890;
  DbElement el(testuint64, ts);

  EXPECT_EQ(el.getType(), DbElementType::UINT64);
  EXPECT_EQ(el.getTimestamp(), ts);
  EXPECT_TRUE(el.hasTimestamp());

  EXPECT_EQ(el.toUint64(), testuint64);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testuint64, ts);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testuint64, ts - 1);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, int64Element) {
  int64_t testint64 = -12345678;
  DbElement el(testint64);

  EXPECT_EQ(el.getType(), DbElementType::INT64);
  EXPECT_ANY_THROW(el.getTimestamp());
  EXPECT_FALSE(el.hasTimestamp());

  EXPECT_EQ(el.toInt64(), testint64);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testint64);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testint64, 1234);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, int64ElementTs) {
  int64_t testint64 = -12345678;
  DbElement::timestamp ts = 1234567890;
  DbElement el(testint64, ts);

  EXPECT_EQ(el.getType(), DbElementType::INT64);
  EXPECT_EQ(el.getTimestamp(), ts);
  EXPECT_TRUE(el.hasTimestamp());

  EXPECT_EQ(el.toInt64(), testint64);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testint64, ts);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testint64, ts - 1);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, floatElement) {
  float testfloat = 0.01;
  DbElement el(testfloat);

  EXPECT_EQ(el.getType(), DbElementType::FLOAT);
  EXPECT_ANY_THROW(el.getTimestamp());
  EXPECT_FALSE(el.hasTimestamp());

  EXPECT_EQ(el.toFloat(), testfloat);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testfloat);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testfloat, 1234);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, floatElementTs) {
  float testfloat = 0.01;
  DbElement::timestamp ts = 1234567890;
  DbElement el(testfloat, ts);

  EXPECT_EQ(el.getType(), DbElementType::FLOAT);
  EXPECT_EQ(el.getTimestamp(), ts);
  EXPECT_TRUE(el.hasTimestamp());

  EXPECT_EQ(el.toFloat(), testfloat);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testfloat, ts);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testfloat, ts - 1);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, doubleElement) {
  double testdouble = 0.01;
  DbElement el(testdouble);

  EXPECT_EQ(el.getType(), DbElementType::DOUBLE);
  EXPECT_ANY_THROW(el.getTimestamp());
  EXPECT_FALSE(el.hasTimestamp());

  EXPECT_EQ(el.toDouble(), testdouble);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testdouble);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testdouble, 1234);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, doubleElementTs) {
  double testdouble = 0.01;
  DbElement::timestamp ts = 1234567890;
  DbElement el(testdouble, ts);

  EXPECT_EQ(el.getType(), DbElementType::DOUBLE);
  EXPECT_EQ(el.getTimestamp(), ts);
  EXPECT_TRUE(el.hasTimestamp());

  EXPECT_EQ(el.toDouble(), testdouble);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toBool());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testdouble, ts);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testdouble, ts - 1);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, boolElement) {
  bool testbool = 0.0001;
  DbElement el(testbool);

  EXPECT_EQ(el.getType(), DbElementType::BOOL);
  EXPECT_ANY_THROW(el.getTimestamp());
  EXPECT_FALSE(el.hasTimestamp());

  EXPECT_EQ(el.toBool(), testbool);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testbool);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testbool, 1234);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, boolElementTs) {
  bool testbool = 0.0001;
  DbElement::timestamp ts = 1234567890;
  DbElement el(testbool, ts);

  EXPECT_EQ(el.getType(), DbElementType::BOOL);
  EXPECT_EQ(el.getTimestamp(), ts);
  EXPECT_TRUE(el.hasTimestamp());

  EXPECT_EQ(el.toBool(), testbool);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBytes());

  DbElement elX(testbool, ts);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testbool, ts - 1);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, bytesElement) {
  std::vector<uint8_t> testbytes = {1, 2, 3, 4, 5};
  DbElement el(testbytes);

  EXPECT_EQ(el.getType(), DbElementType::BYTES);
  EXPECT_ANY_THROW(el.getTimestamp());
  EXPECT_FALSE(el.hasTimestamp());

  EXPECT_EQ(el.toBytes(), testbytes);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());

  DbElement elX(testbytes);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testbytes, 1234);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

//--------------------------------------------------------------------------------------------
TEST(DbElementTest, bytesElementTs) {
  std::vector<uint8_t> testbytes = {1, 2, 3, 4, 5};
  DbElement::timestamp ts = 1234567890;
  DbElement el(testbytes, ts);

  EXPECT_EQ(el.getType(), DbElementType::BYTES);
  EXPECT_EQ(el.getTimestamp(), ts);
  EXPECT_TRUE(el.hasTimestamp());

  EXPECT_EQ(el.toBytes(), testbytes);
  EXPECT_ANY_THROW(el.toString());
  EXPECT_ANY_THROW(el.toUint32());
  EXPECT_ANY_THROW(el.toUint64());
  EXPECT_ANY_THROW(el.toInt32());
  EXPECT_ANY_THROW(el.toInt64());
  EXPECT_ANY_THROW(el.toFloat());
  EXPECT_ANY_THROW(el.toDouble());
  EXPECT_ANY_THROW(el.toBool());

  DbElement elX(testbytes, ts);
  EXPECT_TRUE(elX == el);
  EXPECT_FALSE(elX != el);

  DbElement elY(testbytes, ts - 1);
  EXPECT_FALSE(elY == el);
  EXPECT_TRUE(elY != el);

  DbElement elZ(4641);
  EXPECT_FALSE(elZ == el);
  EXPECT_TRUE(elZ != el);
}

}  // namespace gtests
}  // namespace embDB_database
