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
#include "DefaultTimestamper.hpp"

#include <chrono>

namespace embDB_utilities {
using namespace std::chrono;

//--------------------------------------------------------------------------------------------
DefaultTimestamper::DefaultTimestamper() {}

//--------------------------------------------------------------------------------------------
DefaultTimestamper::~DefaultTimestamper() {}

//--------------------------------------------------------------------------------------------
int64_t DefaultTimestamper::getTimestampMilliseconds() {
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch())
      .count();
}

//--------------------------------------------------------------------------------------------
int64_t DefaultTimestamper::getTimestampSeconds() {
  return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

}  // namespace embDB_utilities
