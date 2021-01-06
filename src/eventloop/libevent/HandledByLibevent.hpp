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

#ifndef _EMBDB_HANDLED_BY_LIBEVENT_HPP_
#define _EMBDB_HANDLED_BY_LIBEVENT_HPP_

#include <event2/event.h>

#include <memory>

#include "EventBaseLibevent.hpp"

namespace embDB_eventloop {

class HandledByLibevent {
 public:
  HandledByLibevent(std::shared_ptr<EventBaseLibevent>& eventbase);
  virtual ~HandledByLibevent();

  struct event* getEvent();
  void setEvent(struct event* ev);

 private:
  struct event* m_event;
  std::shared_ptr<EventBaseLibevent> m_eventBase;
};

}  // namespace embDB_eventloop
#endif
