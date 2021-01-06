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

#include "EventBaseLibevent.hpp"

#include <event2/thread.h>

namespace embDB_eventloop {

//--------------------------------------------------------------------------------------------
EventBaseLibevent::EventBaseLibevent() {
  evthread_use_pthreads();
  m_eventBase = event_base_new();
}

//--------------------------------------------------------------------------------------------
EventBaseLibevent::~EventBaseLibevent() { event_base_free(m_eventBase); }

//--------------------------------------------------------------------------------------------
struct event_base* EventBaseLibevent::getEventBase() {
  return m_eventBase;
}

}  // namespace embDB_eventloop
