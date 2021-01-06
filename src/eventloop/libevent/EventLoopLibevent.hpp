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

#ifndef _EMBDB_EVENT_LOOP_LIBEVENT_HPP_
#define _EMBDB_EVENT_LOOP_LIBEVENT_HPP_

#include <event2/event.h>

#include <list>
#include <unordered_map>

#include "../IEventLoop.hpp"
#include "EventBaseLibevent.hpp"
#include "HandledIoLibevent.hpp"
#include "HandledSignalLibevent.hpp"

namespace embDB_eventloop {

class EventLoopLibevent : public IEventLoop {
 public:
  EventLoopLibevent();
  virtual ~EventLoopLibevent();

  // IEventLoop
  virtual int init() override;
  virtual int reInit() override;
  virtual int registerHandledIo(IHandledIo& handler, int what) override;
  virtual void deregisterHandledIo(IHandledIo& handler) override;
  virtual int registerHandledSignal(IHandledSignal& handler,
                                    SignalHandle s) override;
  virtual int run() override;
  virtual int breakLoop() override;

 private:
  std::shared_ptr<EventBaseLibevent> m_eventBase;
  std::list<std::unique_ptr<HandledIoLibevent> > m_handledIos;
  std::unordered_map<SignalHandle, std::unique_ptr<HandledSignalLibevent> >
      m_handleSignals;
};

}  // namespace embDB_eventloop
#endif
