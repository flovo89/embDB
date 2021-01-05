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

#include <sstream>
#include <vector>

#include "../../../src/database/db-layout/DbLayout.hpp"
// #include "../../mocks/HasherMock.hpp"
// #include "../../mocks/TimestamperMock.hpp"
#include "../../../src/database/db-guard/DbGuard.hpp"
#include "../../../src/utilities/DefaultHasher.hpp"
#include "../../../src/utilities/DefaultMutex.hpp"
#include "../../../src/utilities/DefaultTimestamper.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace embDB_database {
namespace gtests {

class DbLayoutGuardTest : public testing::Test {
 protected:
  void SetUp() override {
    _hasher.reset(new embDB_utilities::DefaultHasher());
    _timestamper.reset(new embDB_utilities::DefaultTimestamper());
    _fileReader.reset(new embDB_fileio::FileReader("testdatabase.protobuf"));
    _fileWriter.reset(new embDB_fileio::FileWriter("testdatabase.protobuf"));

    _layout.reset(new DbLayout(std::move(_fileReader), std::move(_fileWriter),
                               std::move(_hasher), std::move(_timestamper)));
    _mutex.reset(new embDB_utilities::DefaultMutex());

    _guard.reset(new DbGuard(std::move(_layout), std::move(_mutex)));
  }
  void TearDown() override {}

  std::unique_ptr<embDB_utilities::IHasher> _hasher;
  std::unique_ptr<embDB_utilities::ITimestamper> _timestamper;
  std::unique_ptr<embDB_fileio::FileReader> _fileReader;
  std::unique_ptr<embDB_fileio::FileWriter> _fileWriter;

  std::unique_ptr<DbLayout> _layout;
  std::unique_ptr<embDB_utilities::IMutex> _mutex;

  std::unique_ptr<DbGuard> _guard;
};

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, serializationDeserialization) {
  EXPECT_EQ(_guard->serialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, clearAllRowCount) {
  uint32_t count;
  EXPECT_EQ(_guard->clearAll(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->getRowCount(count), DbErrorCode::SUCCESS);
  EXPECT_EQ(count, 0);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, getVersion) {
  uint32_t version;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->getVersion(version), DbErrorCode::SUCCESS);
  EXPECT_EQ(version, 1);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, getUnknownRowItems) {
  std::list<DbElement> elements;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->getAllItems("unknown", elements), DbErrorCode::NOTFOUND);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, addToUnknownRow) {
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->addItem("unknown", DbElement(1234)), DbErrorCode::NOTFOUND);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, addWrongType) {
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow("rowName", DbElementType::STRING, 10),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->addItem("rowName", DbElement(1234)),
            DbErrorCode::TYPEMISMATCH);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, testString) {
  std::string rowName = "whatever";
  std::string elValue = "elvalue";
  std::list<DbElement> elements;
  bool exists;

  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::STRING, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);

  EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
  EXPECT_EQ(elements.size(), 0);

  for (int i = 0; i < 150; i++) {
    std::stringstream ss;
    ss << elValue << i;
    DbElement el(ss.str());
    EXPECT_EQ(_guard->addItem(rowName, el), DbErrorCode::SUCCESS);
    EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
    EXPECT_EQ(elements.size(), ((i + 1) > 100 ? 100 : (i + 1)));
    EXPECT_EQ(elements.back().toString(), ss.str());
  }
  int index = 50;
  for (auto& e : elements) {
    std::stringstream ss;
    ss << elValue << index;
    EXPECT_EQ(e.toString(), ss.str());
    EXPECT_EQ(e.getType(), DbElementType::STRING);
    index++;
  }

  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, testUint32) {
  std::string rowName = "whatever";
  std::list<DbElement> elements;
  bool exists;

  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::UINT32, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);

  EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
  EXPECT_EQ(elements.size(), 0);

  for (uint32_t i = 0; i < 150; i++) {
    DbElement el(i);
    EXPECT_EQ(_guard->addItem(rowName, el), DbErrorCode::SUCCESS);
    EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
    EXPECT_EQ(elements.size(), ((i + 1) > 100 ? 100 : (i + 1)));
    EXPECT_EQ(elements.back().toUint32(), i);
  }
  uint32_t index = 50;
  for (auto& e : elements) {
    EXPECT_EQ(e.toUint32(), index);
    EXPECT_EQ(e.getType(), DbElementType::UINT32);
    index++;
  }

  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, testInt32) {
  std::string rowName = "whatever";
  std::list<DbElement> elements;
  bool exists;

  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::INT32, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);

  EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
  EXPECT_EQ(elements.size(), 0);

  for (int32_t i = 0; i < 150; i++) {
    DbElement el(i);
    EXPECT_EQ(_guard->addItem(rowName, el), DbErrorCode::SUCCESS);
    EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
    EXPECT_EQ(elements.size(), ((i + 1) > 100 ? 100 : (i + 1)));
    EXPECT_EQ(elements.back().toInt32(), i);
  }
  int32_t index = 50;
  for (auto& e : elements) {
    EXPECT_EQ(e.toInt32(), index);
    EXPECT_EQ(e.getType(), DbElementType::INT32);
    index++;
  }

  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, testUint64) {
  std::string rowName = "whatever";
  std::list<DbElement> elements;
  bool exists;

  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::UINT64, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);

  EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
  EXPECT_EQ(elements.size(), 0);

  for (uint64_t i = 0; i < 150; i++) {
    DbElement el(i);
    EXPECT_EQ(_guard->addItem(rowName, el), DbErrorCode::SUCCESS);
    EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
    EXPECT_EQ(elements.size(), ((i + 1) > 100 ? 100 : (i + 1)));
    EXPECT_EQ(elements.back().toUint64(), i);
  }
  uint64_t index = 50;
  for (auto& e : elements) {
    EXPECT_EQ(e.toUint64(), index);
    EXPECT_EQ(e.getType(), DbElementType::UINT64);
    index++;
  }

  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, testInt64) {
  std::string rowName = "whatever";
  std::list<DbElement> elements;
  bool exists;

  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::INT64, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);

  EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
  EXPECT_EQ(elements.size(), 0);

  for (int64_t i = 0; i < 150; i++) {
    DbElement el(i);
    EXPECT_EQ(_guard->addItem(rowName, el), DbErrorCode::SUCCESS);
    EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
    EXPECT_EQ(elements.size(), ((i + 1) > 100 ? 100 : (i + 1)));
    EXPECT_EQ(elements.back().toInt64(), i);
  }
  int64_t index = 50;
  for (auto& e : elements) {
    EXPECT_EQ(e.toInt64(), index);
    EXPECT_EQ(e.getType(), DbElementType::INT64);
    index++;
  }

  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, testFloat) {
  std::string rowName = "whatever";
  std::list<DbElement> elements;
  bool exists;

  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::FLOAT, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);

  EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
  EXPECT_EQ(elements.size(), 0);

  for (float i = 0; i < 150; i = i + 1) {
    DbElement el(i);
    EXPECT_EQ(_guard->addItem(rowName, el), DbErrorCode::SUCCESS);
    EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
    EXPECT_EQ(elements.size(), ((i + 1) > 100 ? 100 : (i + 1)));
    EXPECT_EQ(elements.back().toFloat(), i);
  }
  float index = 50;
  for (auto& e : elements) {
    EXPECT_EQ(e.toFloat(), index);
    EXPECT_EQ(e.getType(), DbElementType::FLOAT);
    index++;
  }

  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, testDouble) {
  std::string rowName = "whatever";
  std::list<DbElement> elements;
  bool exists;

  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::DOUBLE, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);

  EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
  EXPECT_EQ(elements.size(), 0);

  for (double i = 0; i < 150; i = i + 1) {
    DbElement el(i);
    EXPECT_EQ(_guard->addItem(rowName, el), DbErrorCode::SUCCESS);
    EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
    EXPECT_EQ(elements.size(), ((i + 1) > 100 ? 100 : (i + 1)));
    EXPECT_EQ(elements.back().toDouble(), i);
  }
  double index = 50;
  for (auto& e : elements) {
    EXPECT_EQ(e.toDouble(), index);
    EXPECT_EQ(e.getType(), DbElementType::DOUBLE);
    index++;
  }

  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, testBool) {
  std::string rowName = "whatever";
  std::list<DbElement> elements;
  bool exists;
  bool toggle = true;

  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::BOOL, 10),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);

  EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
  EXPECT_EQ(elements.size(), 0);

  for (int i = 0; i < 15; i = i + 1) {
    DbElement el(toggle);
    EXPECT_EQ(_guard->addItem(rowName, el), DbErrorCode::SUCCESS);
    EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
    EXPECT_EQ(elements.size(), ((i + 1) > 10 ? 10 : (i + 1)));
    EXPECT_EQ(elements.back().toBool(), toggle);
    toggle = !toggle;
  }
  int index = 5;
  for (auto& e : elements) {
    EXPECT_EQ(e.toBool(), toggle);
    EXPECT_EQ(e.getType(), DbElementType::BOOL);
    index++;
    toggle = !toggle;
  }

  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, testBytes) {
  std::string rowName = "whatever";
  std::list<DbElement> elements;
  bool exists;

  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::BYTES, 200),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);

  EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
  EXPECT_EQ(elements.size(), 0);

  for (uint8_t i = 0; i < 250; i++) {
    std::vector<uint8_t> vec({i, (uint8_t)(i + 1), (uint8_t)(i + 2),
                              (uint8_t)(i + 3), (uint8_t)(i + 4)});
    DbElement el(vec);
    EXPECT_EQ(_guard->addItem(rowName, el), DbErrorCode::SUCCESS);
    EXPECT_EQ(_guard->getAllItems(rowName, elements), DbErrorCode::SUCCESS);
    EXPECT_EQ(elements.size(), ((i + 1) > 200 ? 200 : (i + 1)));
    EXPECT_EQ(elements.back().toBytes()[0], vec[0]);
  }
  uint8_t index = 50;
  for (auto& e : elements) {
    EXPECT_EQ(e.toBytes()[0], index);
    EXPECT_EQ(e.getType(), DbElementType::BYTES);
    index++;
  }

  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

}  // namespace gtests
}  // namespace embDB_database