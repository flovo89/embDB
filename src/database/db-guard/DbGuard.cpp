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
DbGuard::DbGuard(std::unique_ptr<IDataBaseCircular> circular,
                 std::unique_ptr<IDataBaseLinear> linear,
                 std::unique_ptr<embDB_utilities::IMutex> mutex)
    : m_circular(std::move(circular)),
      m_linear(std::move(linear)),
      m_mutex(std::move(mutex)) {}

//--------------------------------------------------------------------------------------------
DbGuard::~DbGuard() {}

//--------------------------------------------------------------------------------------------
int DbGuard::init() {
  m_mutex->lock();
  int ret = m_circular->init();
  ret += m_linear->init();
  m_mutex->unlock();
  return ret;
}

//--------------------------------------------------------------------------------------------
int DbGuard::deinit() {
  m_mutex->lock();
  int ret = m_circular->deinit();
  ret += m_linear->deinit();
  m_mutex->unlock();
  return ret;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::serialize() {
  m_mutex->lock();
  DbErrorCode err = m_circular->serialize();
  if (err != DbErrorCode::SUCCESS) return err;
  err = m_linear->serialize();
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::clearAll() {
  m_mutex->lock();
  DbErrorCode err = m_circular->clearAll();
  if (err != DbErrorCode::SUCCESS) return err;
  err = m_linear->clearAll();
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getVersionCircular(uint32_t& version) {
  m_mutex->lock();
  DbErrorCode err = m_circular->getVersionCircular(version);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getRowCount(uint32_t& count) {
  m_mutex->lock();
  DbErrorCode err = m_circular->getRowCount(count);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::createRow(std::string name, DbElementType type,
                               uint32_t maxItems) {
  m_mutex->lock();
  DbErrorCode err = m_circular->createRow(name, type, maxItems);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::rowExists(std::string name, bool& exists) {
  m_mutex->lock();
  DbErrorCode err = m_circular->rowExists(name, exists);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::deleteRow(std::string name) {
  m_mutex->lock();
  DbErrorCode err = m_circular->deleteRow(name);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getAllItemsCircular(std::string name,
                                         std::list<DbElement>& elements) {
  m_mutex->lock();
  DbErrorCode err = m_circular->getAllItemsCircular(name, elements);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getItemsBetweenCircular(std::string name, int64_t start,
                                             int64_t end,
                                             std::list<DbElement>& elements) {
  m_mutex->lock();
  DbErrorCode err =
      m_circular->getItemsBetweenCircular(name, start, end, elements);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::addItemCircular(std::string name,
                                     const DbElement& element) {
  m_mutex->lock();
  DbErrorCode err = m_circular->addItemCircular(name, element);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getVersionLinear(uint32_t& version) {
  m_mutex->lock();
  DbErrorCode err = m_linear->getVersionLinear(version);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getAllItemsLinear(std::string name,
                                       std::list<DbElement>& elements) {
  m_mutex->lock();
  DbErrorCode err = m_linear->getAllItemsLinear(name, elements);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::getItemsBetweenLinear(std::string name, int64_t start,
                                           int64_t end,
                                           std::list<DbElement>& elements) {
  m_mutex->lock();
  DbErrorCode err = m_linear->getItemsBetweenLinear(name, start, end, elements);
  m_mutex->unlock();
  return err;
}

//--------------------------------------------------------------------------------------------
DbErrorCode DbGuard::addItemLinear(std::string name, const DbElement& element) {
  m_mutex->lock();
  DbErrorCode err = m_linear->addItemLinear(name, element);
  m_mutex->unlock();
  return err;
}

}  // namespace embDB_database