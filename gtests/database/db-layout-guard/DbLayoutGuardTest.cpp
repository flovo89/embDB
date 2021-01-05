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
#include "../../mocks/HasherMock.hpp"
#include "../../mocks/TimestamperMock.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace embDB_database {
namespace gtests {

class DbLayoutGuardTest : public testing::Test {
 protected:
  void SetUp() override {
    _hasher.reset(new embDB_utilities::gtests::HasherMock());
    _timestamper.reset(new embDB_utilities::gtests::TimestamperMock());
    _fileReader.reset(new embDB_fileio::FileReader("testpath"));
    _fileWriter.reset(new embDB_fileio::FileWriter("testpath"));

    _layout.reset(new DbLayout(std::move(_fileReader), std::move(_fileWriter),
                               std::move(_hasher), std::move(_timestamper)));
  }
  void TearDown() override {}

  std::unique_ptr<embDB_utilities::IHasher> _hasher;
  std::unique_ptr<embDB_utilities::ITimestamper> _timestamper;
  std::unique_ptr<embDB_fileio::FileReader> _fileReader;
  std::unique_ptr<embDB_fileio::FileWriter> _fileWriter;

  std::unique_ptr<DbLayout> _layout;
};

//--------------------------------------------------------------------------------------------
TEST(DbLayoutGuardTest, deserializationError) {}
}  // namespace gtests
}  // namespace embDB_database