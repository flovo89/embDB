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
TEST_F(DbLayoutGuardTest, createRowString) {
  std::string rowName = "whatever";
  bool exists;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::STRING, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);
  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, createRowUint32) {
  std::string rowName = "whatever";
  bool exists;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::UINT32, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);
  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, createRowInt32) {
  std::string rowName = "whatever";
  bool exists;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::INT32, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);
  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, createRowUint64) {
  std::string rowName = "whatever";
  bool exists;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::UINT64, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);
  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, createRowInt64) {
  std::string rowName = "whatever";
  bool exists;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::INT64, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);
  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, createRowFloat) {
  std::string rowName = "whatever";
  bool exists;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::FLOAT, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);
  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, createRowDouble) {
  std::string rowName = "whatever";
  bool exists;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::DOUBLE, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);
  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, createRowBool) {
  std::string rowName = "whatever";
  bool exists;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::BOOL, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);
  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

//--------------------------------------------------------------------------------------------
TEST_F(DbLayoutGuardTest, createRowBytes) {
  std::string rowName = "whatever";
  bool exists;
  EXPECT_EQ(_guard->deserialize(), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->createRow(rowName, DbElementType::BYTES, 100),
            DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_TRUE(exists);
  EXPECT_EQ(_guard->deleteRow(rowName), DbErrorCode::SUCCESS);
  EXPECT_EQ(_guard->rowExists(rowName, exists), DbErrorCode::SUCCESS);
  EXPECT_FALSE(exists);
}

}  // namespace gtests
}  // namespace embDB_database