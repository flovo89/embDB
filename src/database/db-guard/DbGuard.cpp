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

#include "DbGuard.hpp"

namespace embDB_database {

//--------------------------------------------------------------------------------------------
DbGuard::DbGuard(std::unique_ptr<IDataBaseCircular> layout,
                 std::unique_ptr<embDB_utilities::IMutex> mutex)
    : m_layout(std::move(layout)), m_mutex(std::move(mutex)) {}

//--------------------------------------------------------------------------------------------
DbGuard::~DbGuard() {}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::deserialize() {
  m_mutex->lock();
  DbErrorCode err = m_layout->deserialize();
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::serialize() {
  m_mutex->lock();
  DbErrorCode err = m_layout->serialize();
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::clearAll() {
  m_mutex->lock();
  DbErrorCode err = m_layout->clearAll();
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getVersion(uint32_t& version) {
  m_mutex->lock();
  DbErrorCode err = m_layout->getVersion(version);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getRowCount(uint32_t& count) {
  m_mutex->lock();
  DbErrorCode err = m_layout->getRowCount(count);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::createRow(std::string name, DbElementType type,
                               uint32_t maxItems) {
  m_mutex->lock();
  DbErrorCode err = m_layout->createRow(name, type, maxItems);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::rowExists(std::string name, bool& exists) {
  m_mutex->lock();
  DbErrorCode err = m_layout->rowExists(name, exists);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::deleteRow(std::string name) {
  m_mutex->lock();
  DbErrorCode err = m_layout->deleteRow(name);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getAllItems(std::string name,
                                 std::list<DbElement>& elements) {
  m_mutex->lock();
  DbErrorCode err = m_layout->getAllItems(name, elements);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getItemsBetween(std::string name, int64_t start,
                                     int64_t end,
                                     std::list<DbElement>& elements) {
  m_mutex->lock();
  DbErrorCode err = m_layout->getItemsBetween(name, start, end, elements);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::addItem(std::string name, const DbElement& element) {
  m_mutex->lock();
  DbErrorCode err = m_layout->addItem(name, element);
  m_mutex->unlock();
  return err;
}

}  // namespace embDB_database