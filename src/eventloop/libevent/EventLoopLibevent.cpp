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

#include "EventLoopLibevent.hpp"

#include <cassert>

#include "../../exception/Exception.hpp"
#include "../../logging/Loggers.hpp"

namespace embDB_eventloop {

//--------------------------------------------------------------------------------------------
void libeventIoCallback(evutil_socket_t fd, short what, void* data) {
  (void)fd;
  HandledIoLibevent* handle = static_cast<HandledIoLibevent*>(data);

  try {
    if ((what & EV_READ) != 0) handle->getHandledIo().readReady();
    if ((what & EV_WRITE) != 0) handle->getHandledIo().writeReady();
  } catch (const embDB_exception::Exception& ex) {
    LOG_ER() << "IO handle exception: " << ex.getMessage();
    throw ex;
  } catch (const std::exception& ex) {
    LOG_ER() << "IO handle exception: " << ex.what();
    throw ex;
  }
}

//--------------------------------------------------------------------------------------------
void libeventSignalCallback(evutil_socket_t fd, short what, void* data) {
  IHandledSignal* handle = static_cast<IHandledSignal*>(data);

  assert(what & EV_SIGNAL);

  try {
    handle->signalRaised(fd);
  } catch (const embDB_exception::Exception& ex) {
    LOG_ER() << "Signal handler exception:" << ex.getMessage();
    throw ex;
  } catch (const std::exception& ex) {
    LOG_ER() << "Signal handler exception:" << ex.what();
    throw ex;
  }
}

//--------------------------------------------------------------------------------------------
EventLoopLibevent::EventLoopLibevent() : m_eventBase(new EventBaseLibevent()) {}

//--------------------------------------------------------------------------------------------
EventLoopLibevent::~EventLoopLibevent() {}

//--------------------------------------------------------------------------------------------
int EventLoopLibevent::init() { return 0; }

//--------------------------------------------------------------------------------------------
int EventLoopLibevent::reInit() {
  return event_reinit(m_eventBase->getEventBase());
}

//--------------------------------------------------------------------------------------------
int EventLoopLibevent::registerHandledIo(IHandledIo& handler, int what) {
  struct event* ev;
  int libeventWhat = 0;
  std::unique_ptr<HandledIoLibevent> wrappedEvent = nullptr;

  wrappedEvent.reset(new HandledIoLibevent(m_eventBase, handler));
  if ((what & READ) != 0) libeventWhat |= EV_READ;
  if ((what & PERSIST) != 0) libeventWhat |= EV_PERSIST;
  if ((what & WRITE)) libeventWhat |= EV_WRITE;

  ev = event_new(m_eventBase->getEventBase(), handler.getIoHandle(),
                 libeventWhat, libeventIoCallback, wrappedEvent.get());
  if (ev == nullptr) {
    LOG_ER() << "Error creating libevent I/O event!";
    return -1;
  }

  event_add(ev, nullptr);
  wrappedEvent->setEvent(ev);
  m_handledIos.push_back(std::move(wrappedEvent));
  return 0;
}

//--------------------------------------------------------------------------------------------
void EventLoopLibevent::deregisterHandledIo(IHandledIo& handler) {
  for (auto& handledIo : m_handledIos) {
    if (&handledIo->getHandledIo() == &handler) {
      event_del(handledIo->getEvent());
      m_handledIos.remove(handledIo);
      break;
    }
  }
}

//--------------------------------------------------------------------------------------------
int EventLoopLibevent::registerHandledSignal(IHandledSignal& handler,
                                             SignalHandle s) {
  struct event* ev;
  std::unique_ptr<HandledSignalLibevent> wrappedEvent(
      new HandledSignalLibevent(m_eventBase, handler));

  ev = evsignal_new(m_eventBase->getEventBase(), s, libeventSignalCallback,
                    &handler);
  if (ev == nullptr) {
    LOG_ER() << "Error creating libevent signal event!";
    return -1;
  }
  event_add(ev, NULL);
  wrappedEvent->setEvent(ev);

  if (m_handleSignals.find(s) != m_handleSignals.end())
    LOG_WA() << "Signal " << s << " already handled, will be replaced";

  m_handleSignals[s] = std::move(wrappedEvent);

  return 0;
}

//--------------------------------------------------------------------------------------------
int EventLoopLibevent::run() {
  int ret = 0;

  if (m_eventBase == nullptr) {
    LOG_ER() << "Cannot run a non initialized event loop";
    return -1;
  }

  LOG_DB() << "Starting event loop ...";

  ret = event_base_loop(m_eventBase->getEventBase(), 0);

  LOG_DB() << "Event loop broke with code " << ret;

  return ret;
}

//--------------------------------------------------------------------------------------------
int EventLoopLibevent::breakLoop() {
  LOG_DB() << "Breaking event loop ...";

  return event_base_loopbreak(m_eventBase->getEventBase());
}

}  // namespace embDB_eventloop
