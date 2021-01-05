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
#ifndef _EMBDB_DEFAULT_MUTEX_HPP_
#define _EMBDB_DEFAULT_MUTEX_HPP_

#include <mutex>

#include "IMutex.hpp"

namespace embDB_utilities {

class DefaultMutex : public IMutex {
 public:
  DefaultMutex();
  virtual ~DefaultMutex();

  // IMutex
  virtual void lock() override;
  virtual void unlock() override;

 private:
  std::mutex m_mutex;
};

}  // namespace embDB_utilities
#endif
