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

#include "DbLayoutLinear.hpp"

#include "../../exception/Exception.hpp"

namespace embDB_database {

//--------------------------------------------------------------------------------------------
DbLayoutLinear::DbLayoutLinear(
    std::unique_ptr<embDB_utilities::IHasher> hasher,
    std::unique_ptr<embDB_utilities::ITimestamper> timestamper)
    : m_hasher(std::move(hasher)), m_timestamper(std::move(timestamper)) {}

//--------------------------------------------------------------------------------------------
DbLayoutLinear::~DbLayoutLinear() {}

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::init() { return 0; }

//--------------------------------------------------------------------------------------------
int DbLayoutLinear::deinit() { return 0; }

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::serialize() { return DbErrorCode::SUCCESS; }

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::clearAll() { return DbErrorCode::SUCCESS; }

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::getVersionLinear(uint32_t& version) {
  (void)version;

  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::getAllItemsLinear(std::string name,
                                              std::list<DbElement>& elements) {
  (void)name;
  (void)elements;
  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::getItemsBetweenLinear(
    std::string name, int64_t start, int64_t end,
    std::list<DbElement>& elements) {
  (void)name;
  (void)start;
  (void)end;
  (void)elements;
  return DbErrorCode::SUCCESS;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbLayoutLinear::addItemLinear(std::string name,
                                          const DbElement& element) {
  (void)name;
  (void)element;
  return DbErrorCode::SUCCESS;
}

}  // namespace embDB_database