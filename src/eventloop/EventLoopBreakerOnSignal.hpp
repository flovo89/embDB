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

#ifndef _EMBDB_EVENT_LOOP_BREAKER_ON_SIGNAL_HPP_
#define _EMBDB_EVENT_LOOP_BREAKER_ON_SIGNAL_HPP_

#include "IEventLoop.hpp"

namespace embDB_eventloop {

class EventLoopBreakerOnSignal : public IHandledSignal {
 public:
  EventLoopBreakerOnSignal(IEventLoop& el);
  virtual ~EventLoopBreakerOnSignal();

  // IHandledSignal
  virtual void signalRaised(SignalHandle s) override;

 private:
  IEventLoop& m_el;
};

}  // namespace embDB_eventloop
#endif
