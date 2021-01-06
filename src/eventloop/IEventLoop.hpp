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
#ifndef _EMBDB_I_EVENT_LOOP_HPP_
#define _EMBDB_I_EVENT_LOOP_HPP_

#include <memory>

#include "IHandledIo.hpp"
#include "IHandledSignal.hpp"

namespace embDB_eventloop {

class IEventLoop {
 public:
  enum EventType { READ = 0x01, PERSIST = 0x02, WRITE = 0x04 };

  IEventLoop();
  virtual ~IEventLoop();

  virtual int init() = 0;
  virtual int reInit() = 0;
  virtual int registerHandledIo(IHandledIo& handler, int what) = 0;
  virtual void deregisterHandledIo(IHandledIo& handler) = 0;
  virtual int registerHandledSignal(IHandledSignal& handler,
                                    SignalHandle s) = 0;
  virtual int run() = 0;
  virtual int breakLoop() = 0;
};

}  // namespace embDB_eventloop
#endif
