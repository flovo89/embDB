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
#ifndef _EMBDB_GTESTS_MUTEX_MOCK_HPP_
#define _EMBDB_GTESTS_MUTEX_MOCK_HPP_

#include "../../src/utilities/IMutex.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace embDB_utilities {
namespace gtests {

class MutexMock : public IMutex {
 public:
  MOCK_METHOD(void, lock, (), (override));
  MOCK_METHOD(void, unlock, (), (override));
};

}  // namespace gtests
}  // namespace embDB_utilities
#endif
