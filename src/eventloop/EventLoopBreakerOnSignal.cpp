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

#include "EventLoopBreakerOnSignal.hpp"

#include "../logging/Loggers.hpp"

namespace embDB_eventloop {

//--------------------------------------------------------------------------------------------
EventLoopBreakerOnSignal::EventLoopBreakerOnSignal(IEventLoop& el) : m_el(el) {}

//--------------------------------------------------------------------------------------------
EventLoopBreakerOnSignal::~EventLoopBreakerOnSignal() {}

//--------------------------------------------------------------------------------------------
void EventLoopBreakerOnSignal::signalRaised(SignalHandle s) {
  LOG_IN() << "Got signal " << s << " breaking loop ...";
  m_el.breakLoop();
}

}  // namespace embDB_eventloop
