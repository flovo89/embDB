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

#include "HandledByLibevent.hpp"

namespace embDB_eventloop {

//--------------------------------------------------------------------------------------------
HandledByLibevent::HandledByLibevent(
    std::shared_ptr<EventBaseLibevent>& eventbase)
    : m_event(nullptr), m_eventBase(eventbase) {}

//--------------------------------------------------------------------------------------------
HandledByLibevent::~HandledByLibevent() {
  if (m_event != nullptr) event_free(m_event);
}

//--------------------------------------------------------------------------------------------
struct event* HandledByLibevent::getEvent() {
  return m_event;
}

//--------------------------------------------------------------------------------------------
void HandledByLibevent::setEvent(struct event* ev) { m_event = ev; }

}  // namespace embDB_eventloop
