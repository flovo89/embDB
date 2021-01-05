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
#ifndef _EMBDB_GTESTS_TIMESTAMPER_MOCK_HPP_
#define _EMBDB_GTESTS_TIMESTAMPER_MOCK_HPP_

#include "../../src/utilities/ITimestamper.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace embDB_utilities {
namespace gtests {

class TimestamperMock : public ITimestamper {
 public:
  MOCK_METHOD(int64_t, getTimestampMilliseconds, (), (override));
  MOCK_METHOD(int64_t, getTimestampSeconds, (), (override));
};

}  // namespace gtests
}  // namespace embDB_utilities
#endif
