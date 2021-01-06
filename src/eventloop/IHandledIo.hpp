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
#ifndef _EMBDB_I_IO_HANDLER_HPP_
#define _EMBDB_I_IO_HANDLER_HPP_

namespace embDB_eventloop {

typedef int IoHandle;

class IHandledIo {
 public:
  static const IoHandle INVALID_IO_HANDLE;

  IHandledIo();
  virtual ~IHandledIo();

  virtual IoHandle getIoHandle() = 0;
  virtual void readReady();
  virtual void writeReady();
};

}  // namespace embDB_eventloop
#endif
