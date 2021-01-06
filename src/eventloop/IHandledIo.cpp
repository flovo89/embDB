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

#include "IHandledIo.hpp"

#include "../exception/Exception.hpp"

namespace embDB_eventloop {

const IoHandle IHandledIo::INVALID_IO_HANDLE = -1;

//--------------------------------------------------------------------------------------------
IHandledIo::IHandledIo() {}

//--------------------------------------------------------------------------------------------
IHandledIo::~IHandledIo() {}

//--------------------------------------------------------------------------------------------
void IHandledIo::readReady() {
  EMBDB_THROW("Default read ready handler on IO handle");
}

//--------------------------------------------------------------------------------------------
void IHandledIo::writeReady() {
  EMBDB_THROW("Default write ready handler on IO handle");
}

}  // namespace embDB_eventloop
